//
//  app.h
//  oculus_real
//
//  Created by Ethan Li on 3/04/2014.
//
//

#ifndef __oculus_view__app__
#define __oculus_view__app__

#include <iostream>
#include <glew.h>

#include "OculusAppBase.h"
#include "WebCam.h"
#include "OpenGLApp.h"

static const GLfloat vertex_positions[] =
        {
                -0.25f, 0.25f, -0.25f,
                -0.25f, -0.25f, -0.25f,
                0.25f, -0.25f, -0.25f,

                0.25f, -0.25f, -0.25f,
                0.25f, 0.25f, -0.25f,
                -0.25f, 0.25f, -0.25f,

                0.25f, -0.25f, -0.25f,
                0.25f, -0.25f, 0.25f,
                0.25f, 0.25f, -0.25f,

                0.25f, -0.25f, 0.25f,
                0.25f, 0.25f, 0.25f,
                0.25f, 0.25f, -0.25f,

                0.25f, -0.25f, 0.25f,
                -0.25f, -0.25f, 0.25f,
                0.25f, 0.25f, 0.25f,

                -0.25f, -0.25f, 0.25f,
                -0.25f, 0.25f, 0.25f,
                0.25f, 0.25f, 0.25f,

                -0.25f, -0.25f, 0.25f,
                -0.25f, -0.25f, -0.25f,
                -0.25f, 0.25f, 0.25f,

                -0.25f, -0.25f, -0.25f,
                -0.25f, 0.25f, -0.25f,
                -0.25f, 0.25f, 0.25f,

                -0.25f, -0.25f, 0.25f,
                0.25f, -0.25f, 0.25f,
                0.25f, -0.25f, -0.25f,

                0.25f, -0.25f, -0.25f,
                -0.25f, -0.25f, -0.25f,
                -0.25f, -0.25f, 0.25f,

                -0.25f, 0.25f, -0.25f,
                0.25f, 0.25f, -0.25f,
                0.25f, 0.25f, 0.25f,

                0.25f, 0.25f, 0.25f,
                -0.25f, 0.25f, 0.25f,
                -0.25f, 0.25f, -0.25f
        };

using namespace OVR;
using namespace OVR::Util::Render;

class OculusApp : public OpenGLApp, public OculusAppBase {
public:
    OculusApp() {}
    ~OculusApp() {}

private:
    void init();

    virtual void startup();
    virtual void render(double currentTime);
    virtual void renderScene(double currentTime, StereoEyeParams params);

    virtual void shutdown();

private:
    void setupRenderProgram();

    GLuint program;
    GLuint buffer;
    GLuint vao;

    void drawMany(double currentTime);

    void drawOne(double time);

};

#endif /* defined(__oculus_view__app__) */
