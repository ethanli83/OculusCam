//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include <matrix_transform.hpp>
#include "CubesSceneEngine.h"
#include "AppHelper.h"

void CubesSceneEngine::initScene() {
    SceneEngine::initScene();

    GLuint program = glCreateProgram();

    GLuint vs = loadShader("resources/shaders/vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = loadShader("resources/shaders/fs.glsl", GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint modelViewMatrixLocation = (GLuint)glGetUniformLocation(program, "model_view_matrix");
    GLuint projectionMatrixLocation = (GLuint)glGetUniformLocation(program, "projection_matrix");

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    ptrRenderProgram = std::shared_ptr<RenderProgram>(new RenderProgram(program, buffer, vao));

    for (int i = 0; i < 20; i ++) {
        SceneObject obj(modelViewMatrixLocation, projectionMatrixLocation, GL_TRIANGLES, 36);
        obj.ptrRenderProgram = ptrRenderProgram;
        scene.addObject(obj);
    }

    camera.viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
    camera.projectMatrix = glm::perspective(50.0f, (float)winWidth / (float)winHeight, 1.0f, 10000.f);

    renderer->init();
}

void CubesSceneEngine::updateScene(double time) {
    SceneEngine::updateScene(time);
    int i = 0;
    for (SceneObject& obj : scene.objects) {
        float f = (i++) + (float)time * 0.3f;
        glm::mat4 mvMatrix =
                glm::rotate(glm::mat4(1.0f), (float)time * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), (float)time * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::translate(glm::mat4(1.0f), glm::vec3(sinf(2.1f * f) * 2.0f, cosf(1.7f * f) * 2.0f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f));
        obj.transform(mvMatrix);
    }
}

void CubesSceneEngine::renderScene(double time) {
    renderer->preRender(ViewPort(0, 0, winWidth, winHeight));
    renderer->render(scene, camera, time);
    renderer->postRender();
}
