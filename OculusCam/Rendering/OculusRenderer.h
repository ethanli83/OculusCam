//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include <OVR_Device.h>
#include <Util_Render_Stereo.h>
#include <OVR_SensorFusion.h>
#include "BaseRenderer.h"
#include "WebCam.h"

#ifndef __OculusRenderer_H_
#define __OculusRenderer_H_

using namespace OVR;
using namespace OVR::Util::Render;

class OculusRenderer : public BaseRenderer, public MessageHandler {

public:
    OculusRenderer();

    ~OculusRenderer();

    virtual void init();

    virtual void preRender(ViewPort vp) override;

    virtual void render(Scene scene, Camera camera, double time) override;

    virtual void postRender() override;

protected:
    glm::mat4 castMatrix(Matrix4f matrix4f) {
        glm::mat4 matrix;
        for (int i = 0; i < 16; i++)
            matrix[i / 4][i % 4] = matrix4f.M[i % 4][i / 4];
        return matrix;
    }

    int winWidth;
    int winHeight;

    GLint modelViewMatrixLocation;
    GLint projectionMatrixLocation;

    glm::mat4 oculusMatrix;

    StereoConfig stereoConfig;
    SensorFusion sensorFusion;

private:
    void setupRenderProgram();
    void setupRenderTargetBuffer();

    void renderScreen(double time, StereoEyeParams const params);
    void renderStereo(Scene scene, Camera camera, double time, StereoEyeParams const params);

    void updateOculusCamera(Camera camera, StereoEyeParams const aConst);
    void loadCamImageToTex(WebCam cam, GLuint texture);

    GLuint oculusProgramVAO;

    GLuint oculusProgram;

    GLint texCoordLoc;
    GLint positionLoc;

    const glm::mat4 posMatrix = glm::mat4(
            glm::vec4(2, 0, 0, 0),
            glm::vec4(0, 2, 0, 0),
            glm::vec4(0, 0, 0, 0),
            glm::vec4(-1, -1, 0, 1)
    );

    GLint posMatrixLoc;
    GLint texMatrixLoc;

    GLint lensCenterLoc;
    GLint screenCenterLoc;

    GLint scaleLoc;
    GLint scaleInLoc;

    GLint hmdWarpParamLoc;
    GLint chromAbParamLoc;

    GLuint screenRenderingBuffer;

    GLuint renderTargetBuffer;
    GLuint renderResultTexture;

    GLuint leftEyeCamTex;
    GLuint rightEyeCamTex;

    WebCam leftCam;
    WebCam rightCam;

    Camera oculusCamera;

    Ptr<DeviceManager> ptrDeviceManager;
    Ptr<SensorDevice> ptrSensor;
    Ptr<HMDDevice> ptrHmd;

    OVR::HMDInfo hmdInfo;

    glm::vec3 eyePosition;
    float eyeYaw;         // Rotation around Y, CCW positive when looking at RHS (X,Z) plane.
    float eyePitch;       // Pitch. If sensor is plugged in, only read from sensor.
    float eyeRoll;        // Roll, only accessible from Sensor.
    float lastEyeYaw;  // Stores previous Yaw value from to support computing delta.
};


#endif //__OculusRenderer_H_
