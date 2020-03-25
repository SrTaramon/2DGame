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
	vector<int> coordRocks, coordIs;
	vector< Rocks* > vRocks;
	vector< Is* > vIs;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	glm::vec2 babaPos, youPos, flagPos, flagCarPos, winPos;

};


#endif // _SCENE_INCLUDE

