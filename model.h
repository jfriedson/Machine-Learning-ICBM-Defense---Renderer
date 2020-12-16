#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/config.h>

#include "mesh.h"
#include "texture.h"
#include "shader.h"

class Model
{
private:
	Mesh mesh;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

public:
	Model(const char* modelFile, glm::vec3 pos = glm::vec3(0.f), glm::vec3 org = glm::vec3(0.f), glm::vec3 rot = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));

	void setPosition(glm::vec3 position);
	void setOrigin(glm::vec3 origin);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	void setModelMatrix(glm::mat4 modelMatrix);
	void updateModelMatrix();

	void render(Shader* shader, bool tessellate);

	~Model();
};
