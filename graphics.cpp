#include "graphics.h"

Graphics::Graphics(
	const char* title,
	Camera* camera,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	const bool resizable
) :
	window(title, WINDOW_WIDTH, WINDOW_HEIGHT, GL_VERSION_MAJOR, GL_VERSION_MINOR, resizable),
	framebufferWidth(WINDOW_WIDTH),
	framebufferHeight(WINDOW_HEIGHT),
	camera(camera)
{
	this->deltaTime = 0.f;
	this->curTime = 0.f;
	this->lastTime = glfwGetTime();

	this->fov = 60.f;
	this->nearPlane = 0.1f;
	this->farPlane = 500.f;

	ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<GLfloat>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClearDepth(1.f);
}

Graphics::~Graphics()
{
	window.closeWindow();
}

void Graphics::updateInput()
{
	glfwPollEvents();

	if (this->window.getKeys()[GLFW_KEY_ESCAPE] == true)
	{
		this->window.closeWindow();
	}

	curTime = glfwGetTime();
	deltaTime = curTime - lastTime;
	lastTime = curTime;

	camera->keyControl(window.getKeys(), deltaTime);
	camera->mouseControl(window.getMouseDeltaX(), window.getMouseDeltaY(), window.getMouseScroll());
}


void Graphics::updateUniforms(Shader* shader)
{
	shader->setMat4fv(camera->calculateViewMatrix(), "ViewMatrix");

	if (window.getBufferWidth() != this->framebufferWidth ||
		window.getBufferHeight() != this->framebufferHeight)
	{
		this->framebufferWidth = window.getBufferWidth();
		this->framebufferHeight = window.getBufferHeight();

		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<GLfloat>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);
	}
	shader->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	shader->setVec3f(this->camera->getCameraPosition(), "camPos");
	shader->setVec3f(glm::vec3(1.f, 0.f, 0.f), "lightDir");
}
