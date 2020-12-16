#pragma once

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec3 tangent;

	Vertex(glm::vec3 pos, glm::vec2 texCoord, glm::vec3 normal, glm::vec3 tangent) :
		pos(pos),
		texCoord(texCoord),
		normal(normal),
		tangent(tangent)
	{}
};
