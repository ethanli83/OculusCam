//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include <type_ptr.hpp>
#include <matrix_transform.hpp>
#include "OculusRenderer.h"

const glm::vec3 UpVec(0.0f, 1.0f, 0.0f);
const glm::vec3 ForwardVec(0.0f, 0.0f, -1.0f);
const float YawInitial = 3.141592f;

static const GLfloat ScreenQuadVertex[] = {
        0.0f, 1.0f, 0.0f, 1.0f, // BL   x
        1.0f, 0.0f, 1.0f, 0.0f, // TR   |
        0.0f, 0.0f, 0.0f, 0.0f, // TL   x-x
        0.0f, 1.0f, 0.0f, 1.0f, // BL   x-x
        1.0f, 1.0f, 1.0f, 1.0f, // BR     |
        1.0f, 0.0f, 1.0f, 0.0f  // TR     x
};

OculusRenderer::OculusRenderer():
    eyePosition(0.0f, 1.6f, -5.0f),
    eyeYaw(YawInitial), eyePitch(0), eyeRoll(0),
    stereoConfig(),
    leftCam(2), rightCam(0) {

    clearColour.a = 0.0f;
}

OculusRenderer::~OculusRenderer() {
    glDeleteVertexArrays(1, &oculusProgramVAO);
    glDeleteProgram(oculusProgram);
    glDeleteBuffers(1, &screenRenderingBuffer);
    glDeleteFramebuffers(1, &renderTargetBuffer);
    glDeleteTextures(1, &renderResultTexture);
    glDeleteTextures(1, &leftEyeCamTex);
    glDeleteTextures(1, &rightEyeCamTex);

    RemoveHandlerFromDevices();
    ptrSensor.Clear();
    ptrHmd.Clear();
    ptrDeviceManager.Clear();
}

void OculusRenderer::init() {
    BaseRenderer::init();

    const char *detectionMessage;

    ptrDeviceManager = *DeviceManager::Create();
    ptrDeviceManager->SetMessageHandler(this);

    // Release Sensor/HMD in case this is a retry.
    ptrSensor.Clear();
    ptrHmd.Clear();

    ptrHmd = *ptrDeviceManager->EnumerateDevices<HMDDevice>().CreateDevice();
    if (ptrHmd) {
        ptrSensor = *ptrHmd->GetSensor();

        // This will initialize hmdInfo with information about configured IPD,
        // screen size and other variables needed for correct projection.
        // We pass HMD DisplayDeviceName into the renderer to select the
        // correct monitor in full-screen mode.
        if (ptrHmd->GetDeviceInfo(&hmdInfo))
            stereoConfig.SetHMDInfo(hmdInfo);
    }
    else {
        // If we didn't detect an HMD, try to create the sensor directly.
        // This is useful for debugging sensor interaction; it is not needed in
        // a shipping app.
        ptrSensor = *ptrDeviceManager->EnumerateDevices<SensorDevice>().CreateDevice();
    }

    // If there was a problem detecting the Rift, display appropriate message.
    if (!ptrHmd && !ptrSensor)
        detectionMessage = "Oculus Rift not detected.";
    else if (!ptrHmd)
        detectionMessage = "Oculus Sensor detected; HMD Display not detected.";
    else if (!ptrSensor)
        detectionMessage = "Oculus HMD Display detected; Sensor not detected.";
    else if (hmdInfo.DisplayDeviceName[0] == '\0')
        detectionMessage = "Oculus Sensor detected; HMD display EDID not detected.";
    else
        detectionMessage = 0;

    if (detectionMessage) {
        throwError(detectionMessage);
    }

    if (hmdInfo.HResolution > 0) {
        winWidth = hmdInfo.HResolution;
        winHeight = hmdInfo.VResolution;
    }

    if (ptrSensor) {
        // We need to attach sensor to SensorFusion object for it to receive
        // body frame messages and update orientation. sensorFusion.GetOrientation()
        // is used in OnIdle() to orient the view.
        sensorFusion.AttachToSensor(ptrSensor);
        sensorFusion.SetDelegateMessageHandler(this);
        sensorFusion.SetPredictionEnabled(true);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);

    setupRenderProgram();
    setupRenderTargetBuffer();

    leftCam.startCapture();
    rightCam.startCapture();
}

void OculusRenderer::preRender(ViewPort vp) {
    BaseRenderer::preRender(vp);
}

void OculusRenderer::render(Scene scene, Camera camera, double time) {

    switch (stereoConfig.GetStereoMode()) {
        case Stereo_None:
            BaseRenderer::render(scene, camera, time);
            break;

        case Stereo_LeftRight_Multipass:
            renderStereo(scene, camera, time, stereoConfig.GetEyeRenderParams(StereoEye_Left));
            renderStereo(scene, camera, time, stereoConfig.GetEyeRenderParams(StereoEye_Right));
            break;
    }

}

void OculusRenderer::postRender() {
    BaseRenderer::postRender();
}

void OculusRenderer::renderStereo(Scene scene, Camera camera, double time, StereoEyeParams const params) {
    glBindFramebuffer(GL_FRAMEBUFFER, renderTargetBuffer);

    updateOculusCamera(camera, params);

    ViewPort vp(params.VP.x, params.VP.y, params.VP.w, params.VP.h);
    BaseRenderer::preRender(vp);
    BaseRenderer::render(scene, oculusCamera, time);

    renderScreen(time, params);
}

void OculusRenderer::renderScreen(double time, StereoEyeParams const params) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ViewPort vp(params.VP.x, params.VP.y, params.VP.w, params.VP.h);
    BaseRenderer::preRender(vp);

    glUseProgram(oculusProgram);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(oculusProgramVAO);

    float w = float(params.VP.w) / float(winWidth),
            h = float(params.VP.h) / float(winHeight),
            x = float(params.VP.x) / float(winWidth),
            y = float(params.VP.y) / float(winHeight);

    float aspect = float(params.VP.w) / float(params.VP.h);

    DistortionConfig distortion = *params.pDistortion;

    float offsetX = params.Eye == StereoEye_Left ? distortion.XCenterOffset : -distortion.XCenterOffset;

    glm::vec2 lensCenter = glm::vec2(x + (w + offsetX * 0.5f) * 0.5f, y + h * 0.5f);
    glUniform2f(lensCenterLoc, lensCenter[0], lensCenter[1]);

    glm::vec2 screenCenter = glm::vec2(x + w * 0.5f, y + h * 0.5f);
    glUniform2f(screenCenterLoc, screenCenter[0], screenCenter[1]);

    float scaleFactor = 1.0f / (distortion.Scale * 0.76f);

    glUniform2f(scaleLoc, w / 2.0f * scaleFactor, h / 2.0f * scaleFactor * aspect);
    glUniform2f(scaleInLoc, 2.0f / w, 2.0f / h);

    glUniform4f(hmdWarpParamLoc, distortion.K[0], distortion.K[1], distortion.K[2], distortion.K[3]);
    glUniform4f(chromAbParamLoc, distortion.ChromaticAberration[0], distortion.ChromaticAberration[1], distortion.ChromaticAberration[2], distortion.ChromaticAberration[3]);

    glm::mat4 texMatrix(
            glm::vec4(w, 0, 0, 0),
            glm::vec4(0, h, 0, 0),
            glm::vec4(0, 0, 0, 0),
            glm::vec4(x, y, 0, 1)
    );

    glUniformMatrix4fv(posMatrixLoc, 1, GL_FALSE, glm::value_ptr(posMatrix));
    glUniformMatrix4fv(texMatrixLoc, 1, GL_FALSE, glm::value_ptr(texMatrix));

    if (params.Eye == StereoEye_Left) {
        loadCamImageToTex(leftCam, leftEyeCamTex);
        glBindTexture(GL_TEXTURE_2D, leftEyeCamTex);
    } else {
        loadCamImageToTex(rightCam, rightEyeCamTex);
        glBindTexture(GL_TEXTURE_2D, rightEyeCamTex);
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, renderResultTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void OculusRenderer::updateOculusCamera(Camera camera, StereoEyeParams const params) {

    if (ptrSensor) {
        Quatf hmdOrient = sensorFusion.GetOrientation();
        float yaw = 0.0f;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-conversion"
#pragma ide diagnostic ignored "ImplicitIntegerAndEnumConversion"
        hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &eyePitch, &eyeRoll);
#pragma clang diagnostic pop

        eyeYaw += (yaw - lastEyeYaw);
        lastEyeYaw = yaw;
    }

    if (!ptrSensor) {
        const float maxPitch = ((3.1415f / 2) * 0.98f);
        if (eyePitch > maxPitch)
            eyePitch = maxPitch;
        if (eyePitch < -maxPitch)
            eyePitch = -maxPitch;
    }

    glm::mat4 ypyMatrix = glm::rotate(glm::mat4(1.0f), eyeYaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), eyePitch, glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), eyeRoll, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 up = ypyMatrix * UpVec;
    glm::vec3 forward = ypyMatrix * ForwardVec;

    // Minimal head modelling.
    float headBaseToEyeHeight = 0.15f;  // Vertical height of eye from base of head
    float headBaseToEyeProtrusion = 0.09f;  // Distance forward of eye from base of head

    glm::vec3 eyeCenterInHeadFrame(0.0f, headBaseToEyeHeight, -headBaseToEyeProtrusion);
    glm::vec3 rotatedEyeCenter = ypyMatrix * eyeCenterInHeadFrame;
    glm::vec3 shiftedEyePosition = eyePosition + rotatedEyeCenter;
    shiftedEyePosition[1] -= eyeCenterInHeadFrame[1]; // Bring the head back down to original height

    oculusCamera.viewMatrix = glm::lookAt(shiftedEyePosition, shiftedEyePosition + forward, up) *
                              camera.viewMatrix;
    oculusCamera.projectMatrix = castMatrix(params.Projection);
}

void OculusRenderer::setupRenderProgram() {
    oculusProgram = glCreateProgram();

    GLuint vs = loadShader("resources/shaders/oculus.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = loadShader("resources/shaders/oculus.fs.glsl", GL_FRAGMENT_SHADER);

    glAttachShader(oculusProgram, vs);
    glAttachShader(oculusProgram, fs);
    glLinkProgram(oculusProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    glGenVertexArrays(1, &oculusProgramVAO);
    glBindVertexArray(oculusProgramVAO);

    glGenBuffers(1, &screenRenderingBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, screenRenderingBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenQuadVertex), ScreenQuadVertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, screenRenderingBuffer);

    positionLoc = glGetAttribLocation(oculusProgram, "position");
    glEnableVertexAttribArray((GLuint)positionLoc);
    glVertexAttribPointer((GLuint)positionLoc, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, NULL);

    texCoordLoc = glGetAttribLocation(oculusProgram, "texCoord");
    glEnableVertexAttribArray((GLuint)texCoordLoc);
    glVertexAttribPointer((GLuint)texCoordLoc, 2, GL_FLOAT, false, sizeof(GLfloat) * 4, (GLvoid const *) (sizeof(GLfloat) * 2));

    posMatrixLoc = glGetUniformLocation(oculusProgram, "posMatrix");
    texMatrixLoc = glGetUniformLocation(oculusProgram, "texMatrix");

    lensCenterLoc = glGetUniformLocation(oculusProgram, "LensCenter");
    screenCenterLoc = glGetUniformLocation(oculusProgram, "ScreenCenter");

    scaleLoc = glGetUniformLocation(oculusProgram, "Scale");
    scaleInLoc = glGetUniformLocation(oculusProgram, "ScaleIn");

    hmdWarpParamLoc = glGetUniformLocation(oculusProgram, "HmdWarpParam");
    chromAbParamLoc = glGetUniformLocation(oculusProgram, "ChromAbParam");

    glGenTextures(1, &leftEyeCamTex);
    glGenTextures(1, &rightEyeCamTex);
}

void OculusRenderer::setupRenderTargetBuffer() {
    // set up the texture as a storage of the rendering result
    glGenFramebuffers(1, &renderTargetBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderTargetBuffer);

    glGenTextures(1, &renderResultTexture);
    glBindTexture(GL_TEXTURE_2D, renderResultTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, winWidth, winHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderResultTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throwError("GL_FRAMEBUFFER_COMPLETE");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OculusRenderer::loadCamImageToTex(WebCam cam, GLuint texture) {
    cv::Mat frame;
    cam.capture.read(frame);
    flip(frame, frame, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cam.width * 2, cam.height / 2, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}
