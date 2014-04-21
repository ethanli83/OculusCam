//
// Created by Ethan Li on 15/04/2014.
//

#ifndef __OculusAppBase_H_
#define __OculusAppBase_H_

#include <OVR.h>
#include <Util_Render_Stereo.h>

#define GLM_FORCE_RADIANS

#include <glm.hpp>
#include <glm/gtx/transform.hpp>

#include "WebCam.h"

using namespace OVR;
using namespace OVR::Util::Render;

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


class OculusBase {
protected:
    OculusBase() {
        OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
    }

    glm::mat4 getYprMatrix(float yaw, float pitch, float roll) {
        return glm::rotate(yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
               glm::rotate(pitch, glm::vec3(1.0f, 0.0f, 0.0f)) *
               glm::rotate(roll, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::mat4 castMatrix(Matrix4f matrix4f) {
        glm::mat4 matrix;
        for (int i = 0; i < 16; i++)
            matrix[i / 4][i % 4] = matrix4f.M[i % 4][i / 4];
        return matrix;
    }
};

class OculusAppBase : protected OculusBase, public MessageHandler {
protected:
    OculusAppBase() : eyePosition(0.0f, 1.6f, -5.0f), eyeYaw(YawInitial), eyePitch(0), eyeRoll(0), stereoConfig(), leftCam(2), rightCam(0) {
    }

    ~OculusAppBase() {
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

    void initOculus();
    virtual void startup();
    void renderOculusView(double currentTime);
    virtual void renderScene(double currentTime, StereoEyeParams params) = 0;

    void setViewPort(StereoEyeParams const params);
    void clearBuffers(float r, float g, float b, float a, float depth = 1.0f);

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

    void renderStereo(double time, StereoEyeParams const params);
    void renderScreen(double time, StereoEyeParams const params);

    glm::mat4 getOculusViewMatrix();


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

    Ptr<DeviceManager> ptrDeviceManager;
    Ptr<SensorDevice> ptrSensor;
    Ptr<HMDDevice> ptrHmd;

    OVR::HMDInfo hmdInfo;

    glm::vec3 eyePosition;
    float eyeYaw;         // Rotation around Y, CCW positive when looking at RHS (X,Z) plane.
    float eyePitch;       // Pitch. If sensor is plugged in, only read from sensor.
    float eyeRoll;        // Roll, only accessible from Sensor.
    float lastEyeYaw;  // Stores previous Yaw value from to support computing delta.

    void loadCamImageToTex(cv::VideoCapture capture, GLuint texture);

    void loadCamImageToTex(WebCam cam, GLuint texture);
};


#endif //__OculusAppBase_H_
