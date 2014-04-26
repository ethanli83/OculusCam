//
// Created by Ethan Li on 27/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include "Scene.h"
#include "BaseRenderer.h"

#ifndef __SceneEngine_H_
#define __SceneEngine_H_


class SceneEngine {
public:
    ~SceneEngine() {
        delete renderer;
    }

    virtual void initScene() {}
    virtual void updateScene(double time) {}
    virtual void renderScene(double time) {}

protected:
    Scene scene;
    BaseRenderer *renderer;
};


#endif //__SceneEngine_H_
