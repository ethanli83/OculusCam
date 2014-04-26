//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//

#include "RenderProgram.h"

void RenderProgram::applyProgram() {
    glUseProgram(program);
    glBindVertexArray(vao);
}
