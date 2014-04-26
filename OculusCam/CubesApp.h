//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include "CubesSceneEngine.h"
#include "OpenGLApp.h"


#ifndef __CubesApp_H_
#define __CubesApp_H_


class CubesApp : public OpenGLApp {

public:
    static void systemInit();

    virtual void init() override;


    virtual void startup() override;

    virtual void render(double time) override;

private:
    CubesSceneEngine sceneEngine;
};


#endif //__CubesApp_H_
