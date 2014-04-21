#include "OculusApp.h"

#include <glm/gtc/type_ptr.hpp>

using namespace cv;

void OculusApp::init() {
    static const char title[] = "OpenGL SuperBible - Single Point";

    OpenGLApp::init();

    initOculus();

    info.windowWidth = winWidth;
    info.windowHeight = winHeight;

    memcpy(info.title, title, sizeof(title));
}

void OculusApp::startup() {
    OculusAppBase::startup();
    setupRenderProgram();
}

void OculusApp::setupRenderProgram() {
    program = glCreateProgram();

    GLuint vs = loadShader("resources/shaders/vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = loadShader("resources/shaders/fs.glsl", GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    modelViewMatrixLocation = glGetUniformLocation(program, "model_view_matrix");
    projectionMatrixLocation = glGetUniformLocation(program, "projection_matrix");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void OculusApp::render(double currentTime) {
    renderOculusView(currentTime);
}

void OculusApp::renderScene(double currentTime, StereoEyeParams params) {
    setViewPort(params);
    clearBuffers(1.0f, 1.0f, 1.0f, 0.0f);

    glUseProgram(program);
    glBindVertexArray(vao);

    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(castMatrix(params.Projection)));

    //drawOne(currentTime);

    drawMany(currentTime);
}

void OculusApp::drawOne(double time) {
    glm::mat4 mvMatrix = oculusMatrix * glm::translate(glm::vec3(0.0f, 2.0f, -4.0f)) *
            glm::rotate((float)time * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void OculusApp::drawMany(double currentTime) {

    glGenerateMipmap(GL_TEXTURE_2D);

    for (int i = 0; i < 24; i++)
    {
        float f = (float)i + (float)currentTime * 0.3f;
        glm::mat4 mvMatrix = oculusMatrix * glm::translate(glm::vec3(0.0f, 0.0f, -4.0f)) *
                glm::rotate((float)currentTime * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate((float)currentTime * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::translate(glm::vec3(sinf(2.1f * f) * 2.0f, cosf(1.7f * f) * 2.0f, sinf(1.3f * f) * cosf(1.5f * f) * 2.0f));

        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glDisable(GL_TEXTURE_2D);
}

void OculusApp::shutdown() {
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    glDeleteBuffers(1, &buffer);
}
