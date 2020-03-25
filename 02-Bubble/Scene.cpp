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
	baba = NULL;
	you = NULL;
	flag = NULL;
	flagC = NULL;
	win = NULL;
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i] = NULL;
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i] = NULL;
	}
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (baba != NULL)
		delete baba;
	if (you != NULL)
		delete you;
	if (flag != NULL)
		delete flag;
	if (flagC != NULL)
		delete flagC;
	if (win != NULL)
		delete win;
	for (int i = 0; i < vRocks.size(); i++) {
		if (vRocks[i] != NULL)
			delete vRocks[i];
	}
	for (int i = 0; i < vIs.size(); i++) {
		if (vIs[i] != NULL)
			delete vIs[i];
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

	babaPos = map->getBabaPos();
	if (babaPos.x != NULL & babaPos.y != NULL) {
		baba = new Baba();
		baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		baba->setPosition(glm::vec2(babaPos.x * 32, babaPos.y * 32));
		player->setbabaCartell(baba);
		baba->setTileMap(map);
	}

	youPos = map->getYouPos();
	if (youPos.x != NULL & youPos.y != NULL) {
		you = new You();
		you->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		you->setPosition(glm::vec2(youPos.x * 32, youPos.y * 32));
		player->setYou(you);
		you->setTileMap(map);
	}

	flagCarPos = map->getFlagCarPos();
	if (flagCarPos.x != NULL & flagCarPos.y != NULL) {
		flagC = new FlagCar();
		flagC->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		flagC->setPosition(glm::vec2(flagCarPos.x * 32, flagCarPos.y * 32));
		player->setFlagCar(flagC);
		flagC->setTileMap(map);
	}

	winPos = map->getWinPos();
	if (winPos.x != NULL & winPos.y != NULL) {
		win = new Win();
		win->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		win->setPosition(glm::vec2(winPos.x * 32, winPos.y * 32));
		player->setWin(win);
		win->setTileMap(map);
	}

	flagPos = map->getFlagPos();
	if (flagPos.x != NULL & flagPos.y != NULL) {
		flag = new Flagg();
		flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		flag->setPosition(glm::vec2(flagPos.x * 32, flagPos.y * 32));
		player->setFlagg(flag);
		flag->setTileMap(map);
	}

	// Aqui demanar a title map quantes roques shan de creaar i les posicions d'elles
	coordRocks = map->getRockPos();
	for (int i = 0; i < coordRocks.size(); i += 2) {
		Rocks* rock = new Rocks();
		rock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		rock->setPosition(glm::vec2(32 * coordRocks[i], 32 * coordRocks[i + 1]));
		rock->setTileMap(map);
		player->setRocks(rock);
		vRocks.push_back(rock);
	}

	coordIs = map->getIsPos();
	for (int i = 0; i < coordIs.size(); i += 2) {
		Is* is = new Is();
		is->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		is->setPosition(glm::vec2(32 * coordIs[i], 32* coordIs[i + 1]));
		is->setTileMap(map);
		player->setIs(is);
		vIs.push_back(is);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	baba->update(deltaTime);
	you->update(deltaTime);
	flag->update(deltaTime);
	flagC->update(deltaTime);
	win->update(deltaTime);
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->update(deltaTime);
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i]->update(deltaTime);
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
	baba->render();
	you->render();
	flag->render();
	flagC->render();
	win->render();
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->render();
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i]->render();
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



