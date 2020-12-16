#pragma once

#include "model.h"


class Astrobox
{
private:
	Model model;
	Texture texture;

public:
	Astrobox() :
		model("./res/models/astrobox/astrobox.obj")
	{
		texture.LoadTextureLinear("./res/textures/astrobox/starfield.jpg");
	}
	~Astrobox() {}


	void render(Shader* shader)
	{
		model.updateModelMatrix();

		shader->set1i(0, "stars");

		shader->use();

		texture.UseTexture(0);

		model.render(shader, false);

		shader->unuse();
	}
};
