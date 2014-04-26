//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include <glew.h>

#ifndef __RenderProgram_H_
#define __RenderProgram_H_


class RenderProgram {
public:
    RenderProgram(GLuint prog, GLuint bf, GLuint v) : program(prog), buffer(bf), vao(v) {

    }

    ~RenderProgram() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &buffer);
        glDeleteProgram(program);
    }

    void applyProgram();

    GLuint program;
    GLuint buffer;
    GLuint vao;
};


#endif //__RenderProgram_H_
