//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include "SceneEngine.h"
#include "Camera.h"
#include "OculusRenderer.h"

#ifndef __CubeScene_H_
#define __CubeScene_H_

static const float vertice[] =
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


class CubesSceneEngine : public SceneEngine {
public:
    CubesSceneEngine() {
        renderer = new OculusRenderer();
    }

    virtual void initScene() override;
    virtual void updateScene(double time) override;
    virtual void renderScene(double time) override;

    int winWidth;
    int winHeight;

private:
    std::shared_ptr<RenderProgram> ptrRenderProgram;
    Camera camera;
};


#endif //__CubeScene_H_
