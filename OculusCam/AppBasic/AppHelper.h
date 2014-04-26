//
// Created by Ethan Li on 20/04/2014.
//


#include <stdexcept>
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cerrno>

#ifndef __AppHelper_H_
#define __AppHelper_H_


static void throwError(std::string const &error) {
    std::cout << error << std::endl;
    throw std::runtime_error(error);
}

static std::string readFile(const char *filename) {
    std::ifstream file;
    file.open(filename);

    if (!file) {
        throw(errno);
    }

    std::stringstream stream;
    stream << file.rdbuf();

    file.close();
    return stream.str();
}

static GLuint loadShader(const char *shaderFile, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    // Read shader
    std::string text = readFile(shaderFile);
    const char *shaderSrc = text.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader
    std::cout << "Compiling shader." << std::endl;
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    // Check vertex shader
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar compileLog[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, compileLog);
        std::cerr << compileLog << std::endl;
    }

    return shader;
}

static inline glm::vec3 operator *(const glm::mat4 &mat, const glm::vec3 &vec) {
    glm::vec3 result((0));

    for (int c = 0; c < 3; c++) {
        for (int r = 0; r < 3; r++) {
            result[c] += mat[r][c] * vec[r];
        }
    }

    return result;
}

#endif //__AppHelper_H_
