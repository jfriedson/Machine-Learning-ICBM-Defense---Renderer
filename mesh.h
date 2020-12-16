#pragma once

#include <vector>

#include <GL/gl3w.h>

#include "vertex.h"


class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void RenderMesh(bool tessellated);
	void ClearMesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei vertexCount,
			indexCount;
};
