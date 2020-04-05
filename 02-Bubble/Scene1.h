#ifndef _SCENE1_INCLUDE
#define _SCENE1_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene1
{

public:
	Scene1();
	~Scene1();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

