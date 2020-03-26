#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Rocks.h"
#include "Baba.h"
#include "Is.h"
#include "Flagg.h"
#include "You.h"
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


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setRocks(Rocks* rocks);
	void setIs(Is* Is);
	void setPosition(const glm::vec2 &pos);
	void setbabaCartell(Baba* babaCar);
	void setFlagg(Flagg* flag);
	void setYou(You* youu);
	void setFlagCar(FlagCar* flagCar);
	void setWin(Win* winCar);
	void setAnd(And* andCar);
	void setDie(Die* dieCar);
	void setLava(Lava* lavaCar);
	void setPush(Push* pushCar);
	void setLavaTile(LavaTile* lavaTile);
	void setRockCar(RockCar* rockCar);
	void setStop(Stop* stopCar);
	void setWall(Wall* wallCar);
	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	vector< Rocks* > rocks;
	vector< Is* > vIs;
	vector< LavaTile* > vLavas;
	Baba *baba;
	Flagg *flagg;
	You *you;
	FlagCar* flagC;
	Win* win;
	And* and;
	Die* die;
	Lava* lava;
	Push* push;
	RockCar* rockC;
	Stop* stop;
	Wall* wall;

};


#endif // _PLAYER_INCLUDE


