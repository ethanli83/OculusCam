//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include <OVR_Log.h>
#include <OVR_System.h>
#include "CubesApp.h"

void CubesApp::init() {
    OpenGLApp::init();
    sceneEngine.winWidth = info.windowWidth = 1280;
    sceneEngine.winHeight = info.windowHeight = 800;
}

void CubesApp::startup() {
    OpenGLApp::startup();
    sceneEngine.initScene();
}

void CubesApp::render(double time) {
    sceneEngine.updateScene(time);
    sceneEngine.renderScene(time);
}

void CubesApp::systemInit() {
    OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
}
