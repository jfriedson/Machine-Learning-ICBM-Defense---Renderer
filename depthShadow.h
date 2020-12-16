#pragma once

#include "shader.h"


class depthShadow
{
private:
	unsigned int depthMapFBO;
	unsigned int depthMapTex;
	const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

	float near_plane = 1.0f, far_plane = 12.f;
	glm::mat4 lightProjection;

	glm::mat4 lightView;

	glm::mat4 lightSpaceMatrix;

public:
	depthShadow();
	~depthShadow();

	void bindFBO(Shader *shader);
	void updateUniforms(Shader* shader);

	unsigned int getTexID() { return depthMapTex; };
};
