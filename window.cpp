#include "window.h"


void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);


Window::Window(const char* app_name, GLint window_w, GLint window_h, GLint GL_VERSION_MAJOR, GLint GL_VERSION_MINOR, bool resizable) :
	width(window_w), height(window_h)
{
	// glfw
	if (!glfwInit()) {
		std::cerr << "failed to initialize GLFW" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	glfwWindowHint(GLFW_SAMPLES, 4);    // no MSAA
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	//glfwWindowHint(GLFW_REFRESH_RATE, 60);

	// Create the window
	window = glfwCreateWindow(width, height, app_name, NULL, NULL);
	if (!window)
	{
		std::cerr << "could not create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Get buffer size information
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(window);

	// Handle Key + Mouse Input
	glfwSetKeyCallback(window, handleKeyboard);
	glfwSetCursorPosCallback(window, handleMouse);
	glfwSetScrollCallback(window, handleScroll);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	//glfwSetInputMode(mainWindow, GLFW_STICKY_KEYS, GL_FALSE);
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	GLenum error = gl3wInit();
	if (error != GL3W_OK)
	{
		std::cout << "failed to initialize gl3w" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_MULTISAMPLE);		// MSAA
	//glEnable(GL_FRAMEBUFFER_SRGB);	// Gamma correction

	// OPENGL DEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	/**/

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glfwSetWindowUserPointer(this->window, this);

	for (auto& i : this->keys)
		i = false;

	this->last_x = 0.f;
	this->last_y = 0.f;

	this->mouseFirstMoved = false;

	this->mouse_delta_x = 0.f;
	this->mouse_delta_y = 0.f;
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

GLfloat Window::getMouseDeltaX()
{
	GLfloat delta_x = mouse_delta_x;
	mouse_delta_x = 0.f;
	return delta_x;
}

GLfloat Window::getMouseDeltaY()
{
	GLfloat delta_y = mouse_delta_y;
	mouse_delta_y = 0.f;
	return delta_y;
}

GLfloat Window::getMouseScroll()
{
	GLfloat scroll = mouse_scroll;
	mouse_scroll = 0.f;
	return scroll;
}


void Window::handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			casted_window->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			casted_window->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double x_pos, double y_pos)
{
	Window* casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (!casted_window->mouseFirstMoved)
	{
		casted_window->last_x = x_pos;
		casted_window->last_y = y_pos;
		casted_window->mouseFirstMoved = true;
	}

	casted_window->mouse_delta_x += x_pos - casted_window->last_x;
	casted_window->mouse_delta_y += casted_window->last_y - y_pos;

	casted_window->last_x = x_pos;
	casted_window->last_y = y_pos;
}

void Window::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));

	casted_window->mouse_scroll += yoffset;
}

void Window::windowSizeCallback(GLFWwindow* window, int window_width, int window_height)
{
	Window* casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
	casted_window->width = window_width;
	casted_window->height = window_height;
}


void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
