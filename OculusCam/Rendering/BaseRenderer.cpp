//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include <glew.h>
#include <type_ptr.hpp>
#include "BaseRenderer.h"

void BaseRenderer::render(Scene scene, Camera camera, double time) {

    for(SceneObject& obj : scene.objects) {
        obj.applyProgram();
        obj.transform(camera.viewMatrix * obj.transformMatrix);
        obj.project(camera.projectMatrix);
        obj.draw();
    }

}

void BaseRenderer::preRender(ViewPort vp) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CW);
    glDepthFunc(GL_LEQUAL);

    glViewport(vp.x, vp.y, vp.width, vp.height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(vp.x, vp.y, vp.width, vp.height);

    glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
    glClearDepth(clearDepth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
