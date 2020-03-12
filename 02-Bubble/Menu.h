#pragma once

#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:

	void initShaders();

	Quad* quad;
	ShaderProgram texProgram, simpleProgram;
	float currentTime;
	glm::mat4 projection;
	TexturedQuad* wallpaper;
	Texture textura;

};

