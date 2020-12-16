#pragma once

#include "model.h"


class Clouds
{
private:
	Model model;

	Texture cloudsTex;

	GLfloat initial_rot = 45.f;
	GLfloat axis_tilt = 23.5f;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec2 cloud_offset;


public:
	Clouds() :
		model("./res/models/clouds/clouds.obj"),
		position(glm::vec3(0.f, 0.f, 0.f)),
		origin(glm::vec3(0.f, 0.f, 0.f)),
		rotation(glm::vec3(0.f, 0.f, 23.5f)),
		scale(glm::vec3(1.01f))
	{
		cloudsTex.LoadTextureLinear("./res/textures/earth/clouds_8k.jpg");

		cloud_offset = glm::vec2(0.f);

		setModelMatrix();
	}
	~Clouds() {}

	void rotate(float rotate)
	{
		cloud_offset.x -= rotate * .0027f;
		cloud_offset.y += rotate * .00027f;

		if (cloud_offset.x < 0.f)
			cloud_offset.x += 1.f;
		else if (cloud_offset.x >= 1.f)
			cloud_offset.x -= 1.f;

		if (cloud_offset.y < 0.f)
			cloud_offset.y += 1.f;
		else if (cloud_offset.y >= 1.f)
			cloud_offset.y -= 1.f;
	}

	void setModelMatrix()
	{
		// To make the earth rotate properly, rotate y before z
		glm::mat4 ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, origin);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::translate(ModelMatrix, position - origin);
		ModelMatrix = glm::scale(ModelMatrix, scale);

		model.setModelMatrix(ModelMatrix);
	}

	void render(Shader* shader)
	{
		setModelMatrix();

		shader->set1i(0, "cloudsTex");

		shader->setVec2f(cloud_offset, "cloud_offset");

		shader->use();

		cloudsTex.UseTexture(0);

		model.render(shader, false);

		shader->unuse();
	}
};
