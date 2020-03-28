#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Rocks.h"
#include "Player.h"
#include "Baba.h"
#include "Is.h"
#include "You.h"
#include "Flagg.h"
#include "FlagCar.h"
#include "Win.h"
#include "And.h"
#include "Die.h"
#include "Lava.h"
#include "LavaTile.h"
#include "Push.h"
#include "RockCar.h"
#include "Stop.h"
#include "Wall.h"
#include "WallTile.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Baba *baba;
	You *you;
	Flagg *flag;
	FlagCar *flagC;
	Win *win;
	And *and;
	Die* die;
	Lava* lava;
	Push* push;
	RockCar* rockCar;
	Stop* stop;
	Wall* wall;
	vector<int> coordRocks, coordIs, coordLavas, coordWall;
	vector< Rocks* > vRocks;
	vector< Is* > vIs;
	vector< LavaTile* > vLavas;
	vector< WallTile* > vWall;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::vec2 babaPos, youPos, flagPos, flagCarPos, winPos, andPos, diePos, lavaPos, pushPos, rockCarPos, stopPos, wallPos;

};


#endif // _SCENE_INCLUDE

