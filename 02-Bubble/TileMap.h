#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


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
	void free();
	
	int getTileSize() const { return tileSize; }
	int getMapSizex() const { return mapSize.x; }
	int getMapSizey() const { return mapSize.y; }
	vector<int> getRockPos() { return vrocksPos; }
	vector<int> getIsPos() { return visPos; }
	vector<int> getLavasPos() { return vlavaPos; }
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

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize, babaPos, youPos, flagPos, flagCarPos, winPos, andPos, diePos, lavaPos, pushPos, rockCarPos, stopPos, wallPos;
	int tileSize, blockSize, ntilesheet, numberRocks, numberIs, numberLava;
	Texture tilesheet;
	vector< int > vrocksPos, visPos, vlavaPos; //sempre parell, primer y després x
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

};


#endif // _TILE_MAP_INCLUDE


