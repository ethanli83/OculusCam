//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include <type_ptr.hpp>
#include "SceneObject.h"

void SceneObject::applyProgram() {
    ptrRenderProgram->applyProgram();
}

void SceneObject::transform(glm::mat4 matrix) {
    transformMatrix = matrix;
}

void SceneObject::project(glm::mat4 matrix) {
    projectMatrix = matrix;
}

void SceneObject::draw() {
    glUniformMatrix4fv(transformMatrixLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
    glUniformMatrix4fv(projectMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectMatrix));
    glDrawArrays(primitiveType, 0, primitiveCount);
}
