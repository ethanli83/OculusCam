//
//  OpenGLApp.h
//  OpenGLApp
//
//  Created by Ethan Li on 21/04/2014.
//  Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#ifndef __OpenGLApp__OpenGLApp__
#define __OpenGLApp__OpenGLApp__

#include <iostream>
#include <glfw3.h>


struct AppInfo
{
    char title[128];
    int windowWidth;
    int windowHeight;
    int majorVersion;
    int minorVersion;
    int samples;
    union
    {
        struct
        {
            unsigned int    fullScreen  : 1;
            unsigned int    vsync       : 1;
            unsigned int    cursor      : 1;
            unsigned int    stereo      : 1;
            unsigned int    debug       : 1;
        };
        unsigned int        all;
    } flags;
};

class OpenGLApp {
public:
    OpenGLApp() {}
    
    virtual ~OpenGLApp() {}

    virtual int run();

    virtual void init();

    virtual void startup() {}

    virtual void render(double time) {}
    
    virtual void shutdown() {}

    virtual void onResize(int w, int h) {}

    virtual void onKey(int key, int scanCode, int action, int mods) {}

    virtual void onMouseButton(int button, int action, int mods) {}

    virtual void onCursorMove(double x, double y) {}

    virtual void onScroll(double xOffset, double yOffset) {}

    static void getMousePosition(double & x, double & y) {
        glfwGetCursorPos(window, &x, &y);
    }
    
public:


protected:
    AppInfo info;
    static OpenGLApp* app;
    static GLFWwindow* window;

    static void resizeHandler(GLFWwindow* window, int w, int h) {
        app->onResize(w, h);
    }

    static void keyPressHandler(GLFWwindow* window, int key, int scanCode, int action, int mods) {
        app->onKey(key, scanCode, action, mods);
    }

    static void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods) {
        app->onMouseButton(button, action, mods);
    }

    static void cursorMoveHandler(GLFWwindow *window, double x, double y) {
        app->onCursorMove(x, y);
    }

    static void scrollHandler(GLFWwindow *window, double xOffset, double yOffset) {
        app->onScroll(xOffset, yOffset);
    }

};


#endif /* defined(__OpenGLApp__OpenGLApp__) */
