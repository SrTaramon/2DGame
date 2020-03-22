#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
	rocks = NULL;
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i] = NULL;
	}
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (rocks != NULL)
		delete rocks;
	for (int i = 0; i < vRocks.size(); i++) {
		if (vRocks[i] != NULL)
			delete vRocks[i];
	}
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level00.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getMapSizex(), INIT_PLAYER_Y_TILES * map->getMapSizey()));
	player->setTileMap(map);

	rocks = new Rocks();
	rocks->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	rocks->setPosition(glm::vec2(160, 160));
	rocks->setTileMap(map);
	player->setRocks(rocks);

	// Aqui demanar a title map quantes roques shan de creaar i les posicions d'elles
	vRocks.push_back(new Rocks());
	vRocks[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	vRocks[0]->setPosition(glm::vec2(192, 192));
	vRocks[0]->setTileMap(map);
	player->setRocks(vRocks[0]);
	
	vRocks.push_back(new Rocks());
	vRocks[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	vRocks[1]->setPosition(glm::vec2(224, 224));
	vRocks[1]->setTileMap(map);
	player->setRocks(vRocks[1]);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	rocks->update(deltaTime);
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->update(deltaTime);
	}
}

void Scene::render()
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
	rocks->render();
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



