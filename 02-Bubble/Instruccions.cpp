#include "Instruccions.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <irrKlang.h>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

ISoundEngine* engineI = createIrrKlangDevice();


Instruccions::Instruccions() {
	quad = NULL;
	unCop = true;
}

Instruccions::~Instruccions() {
	delete quad;
}

void Instruccions::init()
{
	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	//quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, simpleProgram);
	wallpaper = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	currentTime = 0.0f;

	textura.loadFromFile("images/instruccions.png", TEXTURE_PIXEL_FORMAT_RGB);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

}

void Instruccions::sounds(int id) {


	if (!engineI) {
		return;
	}
	if (id == 0 && engineI) {
		engineI->drop();
	}
	else if (id == 1) {
		if (engineI) {
			engineI->play2D("sounds/Ins.wav", true);
			engineI->setSoundVolume(0.2);
		}
	}
	else if (id == 2) {
		unCop = true;
		engineI = createIrrKlangDevice();
	}
}

void Instruccions::update(int deltaTime)
{
	if (unCop) {
		sounds(1);
		unCop = false;
	}
	currentTime += deltaTime;
}

void Instruccions::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);

	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);


	//modelview = glm::translate(glm::mat4(1.0f), glm::vec3(260.f, 355.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::scale(modelview, glm::vec3(6.f, 1.f, 1.f));
	//modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	wallpaper->render(textura);

}

void Instruccions::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}