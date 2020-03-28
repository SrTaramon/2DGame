#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 32

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
	and = NULL;
	die = NULL;
	lava = NULL;
	push = NULL;
	rockCar = NULL;
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i] = NULL;
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i] = NULL;
	}
	for (int i = 0; i < vLavas.size(); i++) {
		vLavas[i] = NULL;
	}
	for (int i = 0; i < vWall.size(); i++) {
		vWall[i] = NULL;
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
	if (and != NULL)
		delete and;
	if (die != NULL)
		delete die;
	if (lava != NULL)
		delete lava;
	if (push != NULL)
		delete push;
	if (rockCar != NULL)
		delete rockCar;
	for (int i = 0; i < vRocks.size(); i++) {
		if (vRocks[i] != NULL)
			delete vRocks[i];
	}
	for (int i = 0; i < vIs.size(); i++) {
		if (vIs[i] != NULL)
			delete vIs[i];
	}
	for (int i = 0; i < vLavas.size(); i++) {
		if (vLavas[i] != NULL)
			delete vLavas[i];
	}
	for (int i = 0; i < vWall.size(); i++) {
		if (vWall[i] != NULL)
			delete vWall[i];
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

	andPos = map->getAndPos();
	if (andPos.x != NULL & andPos.y != NULL) {
		and = new And();
		and ->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		and ->setPosition(glm::vec2(andPos.x * 32, andPos.y * 32));
		player->setAnd(and);
		and ->setTileMap(map);
	}

	diePos = map->getDiePos();
	if (diePos.x != NULL & diePos.y != NULL) {
		die = new Die();
		die ->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		die ->setPosition(glm::vec2(diePos.x * 32, diePos.y * 32));
		player->setDie(die);
		die ->setTileMap(map);
	}

	lavaPos = map->getLavaPos();
	if (lavaPos.x != NULL & lavaPos.y != NULL) {
		lava = new Lava();
		lava->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		lava->setPosition(glm::vec2(lavaPos.x * 32, lavaPos.y * 32));
		player->setLava(lava);
		lava->setTileMap(map);
	}

	pushPos = map->getPushPos();
	if (pushPos.x != NULL & pushPos.y != NULL) {
		push = new Push();
		push->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		push->setPosition(glm::vec2(pushPos.x * 32, pushPos.y * 32));
		player->setPush(push);
		push->setTileMap(map);
	}

	rockCarPos = map->getRockCarPos();
	if (rockCarPos.x != NULL & rockCarPos.y != NULL) {
		rockCar = new RockCar();
		rockCar->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		rockCar->setPosition(glm::vec2(rockCarPos.x * 32, rockCarPos.y * 32));
		player->setRockCar(rockCar);
		rockCar->setTileMap(map);
	}

	stopPos = map->getStopPos();
	if (stopPos.x != NULL & stopPos.y != NULL) {
		stop = new Stop();
		stop->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		stop->setPosition(glm::vec2(stopPos.x * 32, stopPos.y * 32));
		player->setStop(stop);
		stop->setTileMap(map);
	}

	wallPos = map->getWallPos();
	if (wallPos.x != NULL & wallPos.y != NULL) {
		wall = new Wall();
		wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		wall->setPosition(glm::vec2(wallPos.x * 32, wallPos.y * 32));
		player->setWall(wall);
		wall->setTileMap(map);
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

	coordLavas = map->getLavasPos();
	for (int i = 0; i < coordLavas.size(); i += 2) {
		LavaTile*  lavaTile = new LavaTile();
		lavaTile->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		lavaTile->setPosition(glm::vec2(32 * coordLavas[i], 32 * coordLavas[i + 1]));
		lavaTile->setTileMap(map);
		player->setLavaTile(lavaTile);
		vLavas.push_back(lavaTile);
	}

	coordWall = map->getWallTilePos();
	for (int i = 0; i < coordWall.size(); i += 2) {
		WallTile* wallTile = new WallTile();
		wallTile->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		wallTile->setPosition(glm::vec2(32 * coordWall[i], 32 * coordWall[i + 1]));
		wallTile->setTileMap(map);
		player->setWallTile(wallTile);
		vWall.push_back(wallTile);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (baba != NULL) 
		baba->update(deltaTime);
	if (you != NULL)
		you->update(deltaTime);
	if (flag != NULL)
		flag->update(deltaTime);
	if (flagC != NULL)
		flagC->update(deltaTime);
	if (win != NULL)
		win->update(deltaTime);
	if (and != NULL)
		and->update(deltaTime);
	if (die != NULL)
		die->update(deltaTime);
	if (lava != NULL)
		lava->update(deltaTime);
	if (push != NULL)
		push->update(deltaTime);
	if (rockCar != NULL)
		rockCar->update(deltaTime);
	if (stop != NULL)
		stop->update(deltaTime);
	if (wall != NULL)
		wall->update(deltaTime);
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->update(deltaTime);
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i]->update(deltaTime);
	}
	for (int i = 0; i < vLavas.size(); i++) {
		vLavas[i]->update(deltaTime);
	}
	for (int i = 0; i < vWall.size(); i++) {
		vWall[i]->update(deltaTime);
	}
	player->update(deltaTime);
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
	if (baba != NULL)
		baba->render();
	if (you != NULL)
		you->render();
	if (flag != NULL)
		flag->render();
	if (flagC != NULL)
		flagC->render();
	if (win != NULL)
		win->render();
	if (and != NULL)
		and ->render();
	if (die != NULL)
		die->render();
	if (lava != NULL)
		lava->render();
	if (push != NULL)
		push->render();
	if (rockCar != NULL)
		rockCar->render();
	if (stop != NULL)
		stop->render();
	if (wall != NULL)
		wall->render();
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->render();
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i]->render();
	}
	for (int i = 0; i < vLavas.size(); i++) {
		vLavas[i]->render();
	}
	for (int i = 0; i < vWall.size(); i++) {
		vWall[i]->render();
	}
	player->render();
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



