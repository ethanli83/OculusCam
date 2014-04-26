//
//  main.cpp
//  OculusCam
//
//  Created by Ethan Li on 21/04/2014.
//  Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include "OculusApp.h"
#include "CubesApp.h"

int main(int argc, const char * argv[])
{
//    OculusApp::systemInit();
//    OculusApp app;
//    app.run();

    CubesApp::systemInit();
    CubesApp app;
    app.run();
    return 0;
}

