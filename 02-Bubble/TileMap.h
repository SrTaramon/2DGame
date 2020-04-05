#ifndef _TILEMAP_INCLUDE
#define _TILEMAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Rocks.h"
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


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void update(int deltaTime);
	void free();
	
	int getTileSize() const { return tileSize; }
	int getMapSizex() const { return mapSize.x; }
	int getMapSizey() const { return mapSize.y; }
	vector<int> getRockPos() { return vrocksPos; }
	vector<int> getIsPos() { return visPos; }
	vector<int> getLavasPos() { return vlavaPos; }
	vector<int> getWallTilePos() { return vwallPos; }
	glm::ivec2 getBabaPos() { return babaPos; }
	glm::ivec2 getYouPos() { return youPos; }
	glm::ivec2 getFlagPos() { return flagPos; }
	glm::ivec2 getFlagCarPos() { return flagCarPos; }
	glm::ivec2 getWinPos() { return winPos; }
	glm::ivec2 getAndPos() { return andPos; }
	glm::ivec2 getDiePos() { return diePos; }
	glm::ivec2 getLavaPos() { return lavaPos; }
	glm::ivec2 getPushPos() { return pushPos; }
	glm::ivec2 getRockCarPos() { return rockCarPos; }
	glm::ivec2 getStopPos() { return stopPos; }
	glm::ivec2 getWallPos() { return wallPos; }

	//bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	/*bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;*/
	//bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	//bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	
	void readLvlStates();

	void collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, string &accio) const;
	bool collisionMoveRightLimit(const glm::ivec2& pos, const glm::ivec2& size) const;

	void collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, string& accio) const;
	bool collisionMoveLeftLimit(const glm::ivec2& pos, const glm::ivec2& size) const;

	void collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, string& accio, int* posY) const;
	bool collisionMoveUpLimit(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	void collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, string& accio, int* posY) const;
	bool collisionMoveDownLimit(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

private:
	bool loadLevel(const string &levelFile, ShaderProgram& program);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize, babaPos, youPos, flagPos, flagCarPos, winPos, andPos, diePos, lavaPos, pushPos, rockCarPos, stopPos, wallPos;
	int tileSize, blockSize, ntilesheet, numberRocks, numberIs, numberLava, numberWalls;
	Texture tilesheet;
	vector< int > vrocksPos, visPos, vlavaPos, vwallPos; //sempre parell, primer y després x
	vector<Texture> vtilesheet;
	glm::vec2 tileTexSize;
	int *map;
	bool hihaBaba = false;
	bool hihaRock = false;
	bool hihaRockCar = false;
	bool hihaWall = false;
	bool hihaFlag = false;

	bool hihaLava = false;

	bool hihaIs = false;
	bool hihaAnd = false;
	bool hihaStop = false;
	bool hihaWin = false;
	bool hihaYou = false;
	bool hihaPush = false;
	bool hihaDead = false;
	bool hihaWallTile = false;
	Baba* baba;
	You* you;
	Flagg* flag;
	FlagCar* flagC;
	Win* win;
	And*and;
	Die* die;
	Lava* lava;
	Push* push;
	RockCar* rockCar;
	Stop* stop;
	Wall* wall;
	std::vector< Rocks* > vRocks;
	vector< Is* > vIs;
	vector< And* > vAnd;
	vector< LavaTile* > vLavas;
	vector< WallTile* > vWall;
	enum
	{
		rockSTOP,
		rockPUSH,
		rockNOTHING
	};

	enum {
		flagWIN,
		flagPUSH,
		flagNOTHING,
		flagSTOP
	};

	enum {
		wallSTOP,
		wallPUSH,
		wallNOTHING
	};

	enum {
		lavaSTOP,
		lavaPUSH,
		lavaDIE,
		lavaNOTHING
	};

	int stateRock, stateFlag, stateWall, stateLava;

};


#endif // _TILE_MAP_INCLUDE


