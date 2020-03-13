#pragma once

#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>
#include <GL/glut.h>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Credits
{

public:
	Credits();
	~Credits();

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

