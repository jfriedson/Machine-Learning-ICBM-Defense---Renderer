#pragma once

#include <GL/gl3w.h>

#include <assimp/scene.h>


class Texture
{
public:
	Texture();
	~Texture();

	bool LoadTextureNearest(const char* fileLoc);
	bool LoadTextureNearest(const aiTexture* texture);

	bool LoadTextureLinear(const char* fileLoc);
	bool LoadTextureLinear(const aiTexture* texture);

	void UseTexture(int unit);

	GLuint getTexID() { return textureID; }

private:
	GLuint textureID;
	int width, height, bitDepth;

	std::string fileLocation;
};
