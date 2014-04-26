//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include <glm.hpp>

#ifndef __Camera3D_H_
#define __Camera3D_H_


class Camera {
public:
    glm::mat4 viewMatrix;
    glm::mat4 projectMatrix;
};


#endif //__Camera3D_H_
