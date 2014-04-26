//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include <glm.hpp>
#include <memory>
#include "RenderProgram.h"

#ifndef __SceneObject_H_
#define __SceneObject_H_

class SceneObject {
public:
    SceneObject (GLuint mvMatLoc, GLuint pMatLoc, GLenum pType, int pCount) :
        transformMatrixLoc(mvMatLoc), projectMatrixLoc(pMatLoc),
        primitiveType(pType), primitiveCount(pCount),
        projectMatrix(1.0f), transformMatrix(1.0f) {
    }

    void applyProgram();

    void project(glm::mat4 matrix);

    void transform(glm::mat4 matrix);

    void draw();

    std::shared_ptr<RenderProgram> ptrRenderProgram;

    glm::mat4 transformMatrix;
    glm::mat4 projectMatrix;

    GLuint transformMatrixLoc;
    GLuint projectMatrixLoc;

    GLenum primitiveType;
    int primitiveCount;
};


#endif //__SceneObject_H_
