#pragma once

#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(const char* app_name, GLint window_w = 800, GLint window_h = 600, GLint GL_VERSION_MAJOR = 4, GLint GL_VERSION_MINOR = 6, bool resizable = true);
	~Window();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	GLint getWidth() { return width; }
	GLint getHeight() { return height; }

	void closeWindow() { glfwSetWindowShouldClose(this->window, GLFW_TRUE); }
	bool windowAlive() { return !glfwWindowShouldClose(window); }

	GLFWwindow* getWindow() { return this->window; }

	bool* getKeys() { return keys; }
	GLfloat getMouseDeltaX();
	GLfloat getMouseDeltaY();
	GLfloat getMouseScroll();

	void swapBuffers() { glfwSwapBuffers(window); }

private:
	GLFWwindow* window;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat last_x;
	GLfloat last_y;
	GLfloat mouse_delta_x;
	GLfloat mouse_delta_y;
	bool mouseFirstMoved;

	GLfloat mouse_scroll;


	static void handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double x_pos, double y_pos);
	static void handleScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void windowSizeCallback(GLFWwindow* window, int window_width, int window_height);
};
