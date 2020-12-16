#pragma once

#include <string.h>
#include <cmath>
#include <vector>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "camera.h"
#include "shader.h"


class Graphics
{
private:
    Window window;
    int framebufferWidth;
    int framebufferHeight;

    GLfloat deltaTime;
    GLfloat curTime;
    GLfloat lastTime;

    Camera *camera;

    glm::mat4 ProjectionMatrix;
    GLfloat fov;
    GLfloat nearPlane;
    GLfloat farPlane;

public:
    Graphics(
        const char* title, Camera *camera,
        const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
        const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
        bool resizable
    );
    ~Graphics();

    bool* keys() { return this->window.getKeys(); }

    void closeWindow() { this->window.closeWindow(); }
    bool windowAlive() { return this->window.windowAlive(); }

    Window* getWindow() { return &this->window; }
    GLFWwindow* getGLFWwindow() { return this->window.getWindow(); }

    void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void render() { this->window.swapBuffers(); }

    void updateInput();

    void updateUniforms(Shader* shader);
};
