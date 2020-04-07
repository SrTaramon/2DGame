#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene4.h"
#include "Game.h"

#define SCREEN_X 32
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 1


Scene4::Scene4()
{
	map = NULL;
	player = NULL;
}

Scene4::~Scene4()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene4::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->setLvl(4);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 4);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 32, INIT_PLAYER_Y_TILES * 32));
	player->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene4::update(int deltaTime)
{
	currentTime += deltaTime;
	map->update(deltaTime);
	player->update(deltaTime);
}

void Scene4::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
}

void Scene4::initShaders()
{
	Shader vShader, fShader;

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
	vShader.free();
	fShader.free();
}



