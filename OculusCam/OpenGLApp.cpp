//
//  OpenGLApp.cpp
//  OpenGLApp
//
//  Created by Ethan Li on 21/04/2014.
//  Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include <thread>
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include "OpenGLApp.h"

OpenGLApp *OpenGLApp::app = 0;
GLFWwindow *OpenGLApp::window = 0;

void OpenGLApp::init() {
    strcpy(info.title, "SuperBible6 Example");
    info.windowWidth = 800;
    info.windowHeight = 600;

    info.majorVersion = 3;
    info.minorVersion = 2;

    info.samples = 0;
    info.flags.all = 0;
    info.flags.cursor = 1;

#ifdef _DEBUG
     info.flags.debug = 1;
#endif
}

int OpenGLApp::run() {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    init();

    app = this;

    /* Create a windowed mode window and its OpenGL context */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif /* _DEBUG */

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, info.samples);
    glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
    
    if (info.flags.fullScreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        info.windowWidth = mode->width;
        info.windowHeight = mode->height;

        window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, monitor, NULL);
        glfwSwapInterval((int) info.flags.vsync);
    }
    else {
        window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, NULL, NULL);
    }

    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }
    
    glfwSetWindowSizeCallback(window, resizeHandler);

    glfwSetKeyCallback(window, keyPressHandler);

    glfwSetMouseButtonCallback(window, mouseButtonHandler);

    glfwSetCursorPosCallback(window, cursorMoveHandler);

    glfwSetScrollCallback(window, scrollHandler);

    glfwSetInputMode(window, GLFW_CURSOR, info.flags.cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    info.flags.stereo = (glfwGetWindowAttrib(window, GLFW_STEREO) ? 1 : 0);

    startup();

    /* Loop until the user closes the window */
    bool running = true;
    while (!glfwWindowShouldClose(window) && running) {
        render(glfwGetTime());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);

        const std::chrono::milliseconds dura(15);
        std::this_thread::sleep_for(dura);
    }

    shutdown();

    glfwTerminate();
    return 0;
}
