//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include "Scene.h"
#include "Camera.h"

#ifndef __OpenGLRenderer_H_
#define __OpenGLRenderer_H_

class ViewPort {
public:
    ViewPort(int x, int y, int w, int h) :
        x(x), y(y), width(w), height(h) {

    }

    int x;
    int y;
    int width;
    int height;
};

class BaseRenderer {
public:
    BaseRenderer() : clearColour((1.0f, 1.0f, 1.0f, 1.0f)), clearDepth(1) {

    }

    virtual void init() {}
    virtual void preRender(ViewPort vp);
    virtual void render(Scene scene, Camera camera, double time = 0);
    virtual void postRender() {}

protected:
    glm::vec4 clearColour;
    int clearDepth;
};


#endif //__OpenGLRenderer_H_
