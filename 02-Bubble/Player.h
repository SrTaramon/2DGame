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
	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	vector< Rocks* > rocks;
	vector< Is* > vIs;
	Baba *baba;
	Flagg *flagg;
	You *you;
	FlagCar* flagC;
	Win* win;

};


#endif // _PLAYER_INCLUDE


