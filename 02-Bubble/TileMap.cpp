#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	vtilesheet[1].use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line); // 3a linia
	sstream.str(line);
	sstream >> tileSize >> blockSize;

	getline(fin, line); // 4a linia
	sstream.str(line);
	sstream >> ntilesheet;

	vtilesheet = *new vector<Texture>(ntilesheet + 1);
	for (int i = 0; i < ntilesheet; i++) {
		getline(fin, line);
		sstream.str(line);
		sstream >> tilesheetFile;
		vtilesheet[i].loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
		vtilesheet[i].setWrapS(GL_CLAMP_TO_EDGE);
		vtilesheet[i].setWrapT(GL_CLAMP_TO_EDGE);
		vtilesheet[i].setMinFilter(GL_NEAREST);
		vtilesheet[i].setMagFilter(GL_NEAREST);
	}

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	// Quins cartells hem de pintar?
	// Baba
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaBaba;
	// Rock
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaRock;
	// Wall
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaWall;
	// Flag
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaFlag;
	// Lava
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaLava;
	// Is
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaIs;
	// And
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaAnd;
	// Stop
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaStop;
	// Win
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaWin;
	// You
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaYou;
	// Push
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaPush;
	// Dead
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaDead;
	// RockCar
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaRockCar;
	// WallTile
	getline(fin, line);
	sstream.str(line);
	sstream >> hihaWallTile;

	if (hihaRock) {
		//roques
		getline(fin, line);
		sstream.str(line);
		sstream >> numberRocks;

		vrocksPos = *new vector<int>(numberRocks * 2);
		for (int i = 0; i < vrocksPos.size() - 1; i += 2) {
			int x, y;
			getline(fin, line);
			sstream.str(line);
			sstream >> y >> x;
			vrocksPos[i] = x;
			vrocksPos[i + 1] = y;
		}
	}

	if (hihaBaba) {
		//cartell Baba
		getline(fin, line);
		sstream.str(line);
		sstream >> babaPos.y >> babaPos.x;
	}

	if (hihaYou) {
		//cartell You
		getline(fin, line);
		sstream.str(line);
		sstream >> youPos.y >> youPos.x;
	}

	if (hihaFlag) {
		//cartell Flag
		getline(fin, line);
		sstream.str(line);
		sstream >> flagCarPos.y >> flagCarPos.x;
	}

	if (hihaWin) {
		//cartell Win
		getline(fin, line);
		sstream.str(line);
		sstream >> winPos.y >> winPos.x;
	}

	if (hihaIs) {
		//cartells Is
		getline(fin, line);
		sstream.str(line);
		sstream >> numberIs;

		visPos = *new vector<int>(numberIs * 2);
		for (int i = 0; i < visPos.size() - 1; i += 2) {
			int x, y;
			getline(fin, line);
			sstream.str(line);
			sstream >> y >> x;
			visPos[i] = x;
			visPos[i + 1] = y;
		}
	}

	if (hihaFlag) {
		//Flag
		getline(fin, line);
		sstream.str(line);
		sstream >> flagPos.y >> flagPos.x;
	}

	if (hihaAnd) {
		//Cartell And
		getline(fin, line);
		sstream.str(line);
		sstream >> andPos.y >> andPos.x;
	}

	if (hihaDead) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> diePos.y >> diePos.x;
	}

	if (hihaLava) {
		//Cartell Lava
		getline(fin, line);
		sstream.str(line);
		sstream >> lavaPos.y >> lavaPos.x;

		//lavas
		getline(fin, line);
		sstream.str(line);
		sstream >> numberLava;

		vlavaPos = *new vector<int>(numberLava * 2);
		for (int i = 0; i < vlavaPos.size() - 1; i += 2) {
			int x, y;
			getline(fin, line);
			sstream.str(line);
			sstream >> y >> x;
			vlavaPos[i] = x;
			vlavaPos[i + 1] = y;
		}
	}

	if (hihaPush) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> pushPos.y >> pushPos.x;
	}

	if (hihaRockCar) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> rockCarPos.y >> rockCarPos.x;
	}

	if (hihaStop) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> stopPos.y >> stopPos.x;
	}

	if (hihaWall) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> wallPos.y >> wallPos.x;
	}

	if (hihaWallTile) {
		//cartells Is
		getline(fin, line);
		sstream.str(line);
		sstream >> numberWalls;

		vwallPos = *new vector<int>(numberWalls * 2);
		for (int i = 0; i < vwallPos.size() - 1; i += 2) {
			int x, y;
			getline(fin, line);
			sstream.str(line);
			sstream >> y >> x;
			vwallPos[i] = x;
			vwallPos[i + 1] = y;
		}
	}

	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / vtilesheet[1].width(), 0.5f / vtilesheet[1].height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%3) / tilesheetSize.x, float((tile-1)/3) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (*posY) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y - size.y <= 4)
			{
				//*posY = (tileSize * y) + size.y;
				return true;
			}
		}
	}

	return false;
}






























