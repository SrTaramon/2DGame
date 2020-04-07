#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <irrKlang.h>

#pragma comment(lib, "irrKlang.lib")


using namespace irrklang;
using namespace std;

ISoundEngine* engineT = createIrrKlangDevice();

#define SCREEN_X 32
#define SCREEN_Y 32

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	/*baba = NULL;
	you = NULL;
	flag = NULL;
	flagC = NULL;
	win = NULL;
	and = NULL;
	die = NULL;
	lava = NULL;
	push = NULL;
	rockCar = NULL;
	stop = NULL;
	wall = NULL;
	

	vector< Rocks* > vRocks;
	vector< Is* > vIs;
	vector< LavaTile* > vLavas;
	vector< WallTile* > vWall;*/
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	hihaLava = false;
	hihaFlag = false;
	flagC = NULL;
	hihaBaba = false;
	hihaYou = false;
	hihaFlag = false;
	hihaWin = false;
	hihaAnd = false;
	hihaDead = false;
	hihaLava = false;
	hihaPush = false;
	hihaRockCar = false;
	hihaStop = false;
	hihaWall = false;


	baba = NULL;
	you = NULL;
	flag = NULL;
	flagC = NULL;
	win = NULL;
	//and = NULL;
	die = NULL;
	lava = NULL;
	push = NULL;
	rockCar = NULL;
	stop = NULL;
	wall = NULL;

	loadLevel(levelFile, program);
	prepareArrays(minCoords, program);
	//Aixo s'ha de fer al moment de lleguir el nivell
	readLvlStates();

}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
	for (int i = 0; i < vLavas.size(); i++) {
		if (vLavas[i] != NULL)
			delete vLavas[i];
	}
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

	if (hihaBaba != false)
		baba->render();

	if (hihaYou != false)
		you->render();
		flagC->render();
	if (hihaLava != false)
		lava->render();
	/*	
		
			*/
	if (hihaFlag != false)
		flag->render();
	if (hihaWin != false)
		win->render();
	//if (hihaAnd != false)
	//	and ->render();
	if (hihaDead != false)
		die->render();

	if (hihaPush != false)
		push->render();
	if (hihaRockCar != false)
		rockCar->render();
	if (hihaStop != false)
		stop->render();
	if (hihaWall != false)
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
	if (hihaAnd) {
		for (int i = 0; i < vAnd.size(); i++) {
			vAnd[i]->render();
		}
	}
}

void TileMap::update(int deltaTime) {

	if (hihaBaba != false)
		baba->update(deltaTime);

	if (hihaYou != false)
		you->update(deltaTime);

	if (hihaFlag != false)
		flag->update(deltaTime);
		flagC->update(deltaTime);
		/*
		
			
			
	}*/
		if (hihaLava != false) {
			lava->update(deltaTime);
			for (int i = 0; i < vLavas.size(); i++) {
				vLavas[i]->update(deltaTime);
			}
		}

		if (hihaAnd) {
			for (int i = 0; i < vAnd.size(); i++) {
				vAnd[i]->update(deltaTime);
			}
		}

	if (hihaWin != false)
		win->update(deltaTime);
	//if (hihaAnd != false)
	//	and ->update(deltaTime);
	if (hihaDead != false)
		die->update(deltaTime);

	if (hihaPush != false)
		push->update(deltaTime);
	if (hihaRockCar != false)
		rockCar->update(deltaTime);
	if (hihaStop != false)
		stop->update(deltaTime);
	if (hihaWall != false)
		wall->update(deltaTime);
	for (int i = 0; i < vRocks.size(); i++) {
		vRocks[i]->update(deltaTime);
	}
	for (int i = 0; i < vIs.size(); i++) {
		vIs[i]->update(deltaTime);
	}

	for (int i = 0; i < vWall.size(); i++) {
		vWall[i]->update(deltaTime);
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile, ShaderProgram& program)
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

	/*
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

		for (int i = 0; i < vrocksPos.size(); i += 2) {
			Rocks* rock = new Rocks();
			rock->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
			rock->setPosition(glm::vec2(32 * vrocksPos[i], 32 * vrocksPos[i + 1]));
			vRocks.push_back(rock);
		}
	}

	if (hihaBaba) {
		//cartell Baba
		getline(fin, line);
		sstream.str(line);
		sstream >> babaPos.y >> babaPos.x;

		baba = new Baba();
		baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		baba->setPosition(glm::vec2(babaPos.x * 32, babaPos.y * 32));
	}

	if (hihaYou) {
		//cartell You
		getline(fin, line);
		sstream.str(line);
		sstream >> youPos.y >> youPos.x;

		you = new You();
		you->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		you->setPosition(glm::vec2(youPos.x * 32, youPos.y * 32));
	}

	if (hihaFlag) {
		//cartell Flag
		getline(fin, line);
		sstream.str(line);
		sstream >> flagCarPos.y >> flagCarPos.x;

		flagC = new FlagCar();
		flagC->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		flagC->setPosition(glm::vec2(flagCarPos.x * 32, flagCarPos.y * 32));
	}

	if (hihaWin) {
		//cartell Win
		getline(fin, line);
		sstream.str(line);
		sstream >> winPos.y >> winPos.x;

		win = new Win();

		win->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		win->setPosition(glm::vec2(winPos.x * 32, winPos.y * 32));

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

		for (int i = 0; i < visPos.size(); i += 2) {
			Is* is = new Is();
			is->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
			is->setPosition(glm::vec2(32 * visPos[i], 32 * visPos[i + 1]));
			vIs.push_back(is);
		}
	}

	if (hihaFlag) {
		//Flag
		getline(fin, line);
		sstream.str(line);
		sstream >> flagPos.y >> flagPos.x;

		flag = new Flagg();
		flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		flag->setPosition(glm::vec2(flagPos.x * 32, flagPos.y * 32));
	}

	if (hihaAnd) {
		//Cartell And
		getline(fin, line);
		sstream.str(line);
		sstream >> andPos.y >> andPos.x;

		and = new And();
		and ->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		and ->setPosition(glm::vec2(andPos.x * 32, andPos.y * 32));
	}

	if (hihaDead) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> diePos.y >> diePos.x;

		die = new Die();
		die->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		die->setPosition(glm::vec2(diePos.x * 32, diePos.y * 32));
	}

	if (hihaLava) {
		//Cartell Lava
		getline(fin, line);
		sstream.str(line);
		sstream >> lavaPos.y >> lavaPos.x;

		lava = new Lava();
		lava->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		lava->setPosition(glm::vec2(lavaPos.x * 32, lavaPos.y * 32));

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


		for (int i = 0; i < vlavaPos.size(); i += 2) {
			LavaTile* lavaTile = new LavaTile();
			lavaTile->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
			lavaTile->setPosition(glm::vec2(32 * vlavaPos[i], 32 * vlavaPos[i + 1]));
			vLavas.push_back(lavaTile);
		}
	}

	if (hihaPush) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> pushPos.y >> pushPos.x;

		push = new Push();
		push->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		push->setPosition(glm::vec2(pushPos.x * 32, pushPos.y * 32));
	}

	if (hihaRockCar) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> rockCarPos.y >> rockCarPos.x;

		rockCar = new RockCar();
		rockCar->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		rockCar->setPosition(glm::vec2(rockCarPos.x * 32, rockCarPos.y * 32));
	}

	if (hihaStop) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> stopPos.y >> stopPos.x;

		stop = new Stop();
		stop->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		stop->setPosition(glm::vec2(stopPos.x * 32, stopPos.y * 32));
	}

	if (hihaWall) {
		//Cartell Die
		getline(fin, line);
		sstream.str(line);
		sstream >> wallPos.y >> wallPos.x;

		wall = new Wall();
		wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
		wall->setPosition(glm::vec2(wallPos.x * 32, wallPos.y * 32));
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

		for (int i = 0; i < vwallPos.size(); i += 2) {
			WallTile* wallTile = new WallTile();
			wallTile->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
			wallTile->setPosition(glm::vec2(32 * vwallPos[i], 32 * vwallPos[i + 1]));
			vWall.push_back(wallTile);
		}

	}
	*/

	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else {
				// ------------------------------------------------- OBJECTES -----------------------------------------------
				if (tile == '1') map[j * mapSize.x + i] = tile - int('0'); // Limits
				else if (tile == '2') { // Flag nomes 1

					hihaFlag = true;
					flag = new Flagg();
					flag->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					flag->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 2;
				}
				else if (tile == '3') { // Wall poden haver molts

					hihaWallTile = true;
					WallTile* wa = new WallTile();
					wa->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					wa->setPosition(glm::vec2(32 * i, 32 * j));
					vWall.push_back(wa);
					map[j * mapSize.x + i] = 3;
				}
				else if (tile == '4') { // Rocks poden haver molts

					hihaRock = true;
					Rocks* rock = new Rocks();
					rock->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					rock->setPosition(glm::vec2(32 * i, 32 * j));
					vRocks.push_back(rock);
					map[j * mapSize.x + i] = 4;
				}
				else if (tile == '5') { // Lava poden haver molts

					hihaLava = true;
					LavaTile* lavaTile = new LavaTile();
					lavaTile->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					lavaTile->setPosition(glm::vec2(32 * i, 32 * j));
					vLavas.push_back(lavaTile);
					map[j * mapSize.x + i] = 5;

				} // --------------------------------------------- CARTELLS ---------------------------------------------
				else if (tile == 'I') { // Cartell IS

					hihaIs = true;
					Is* is = new Is();
					is->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					is->setPosition(glm::vec2(32 * i, 32 * j));
					vIs.push_back(is);
					map[j * mapSize.x + i] = 6;
				}
				else if (tile == 'A') { // Cartell AND
					
					hihaAnd = true;
					And* an = new And();
					an->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					an->setPosition(glm::vec2(32 * i, 32 * j));
					vAnd.push_back(an);
					map[j * mapSize.x + i] = 7;
				}
				else if (tile == 'B') { // Cartell BABA

					hihaBaba = true;
					baba = new Baba();
					baba->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					baba->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 8;
				}
				else if (tile == 'Y') { // Cartell YOU

					hihaYou = true;
					you = new You();
					you->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					you->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 9;
				}
				else if (tile == 'F') { // Cartell FLAG

					hihaFlag = true;
					flagC = new FlagCar();
					flagC->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					flagC->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 10;
				}
				else if (tile == 'W') { // Cartell WIN

					hihaWin = true;
					win = new Win();
					win->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					win->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 11;
				}
				else if (tile == 'w') { // Cartell WALL

					hihaWall = true;
					wall = new Wall();
					wall->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					wall->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 12;
				}
				else if (tile == 'S') { // Cartell WALL

					hihaStop = true;
					stop = new Stop();
					stop->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					stop->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 13;
				}
				else if (tile == 'L') { // Cartell WALL

					hihaLava = true;
					lava = new Lava();
					lava->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					lava->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 14;
				}
				else if (tile == 'D') { // Cartell WALL

					hihaDead = true;
					die = new Die();
					die->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					die->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 15;
				}
				else if (tile == 'R') { // Cartell WALL

					hihaRockCar = true;
					rockCar = new RockCar();
					rockCar->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					rockCar->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 16;
				}
				else if (tile == 'P') { // Cartell WALL

					hihaPush = true;
					push = new Push();
					push->init(glm::ivec2(SCREEN_X, SCREEN_Y), program);
					push->setPosition(glm::vec2(32 * i, 32 * j));
					map[j * mapSize.x + i] = 17;
				}

			}
				
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
			if(tile == 1)
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

/*
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
*/
/*
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
}*/

/*
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
*/
/*
bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y - size.y <= 4)
			{
				*posY = (tileSize * (y+1)) ;
				return true;
			}
		}
	}

	return false;
}
*/

bool TileMap::collisionMoveRightLimit(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 1)
			return true;
	}

	return false;
}

void TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, string &accio) 
{	

	accio = "PUSH";
	if (collisionMoveRightLimit(glm::vec2(pos.x + 2, pos.y), size)) {
		accio = "STOP";
		return;
	}
	else {
		// --------------------------------------------------- CARTELLS ------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaIs) {
			int numIs = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vIs.size(); i++) {
				if (vIs[i]->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {
					
					// Mira limits
					if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaAnd) { // Cartell AND
						for (int j = 0; j < vAnd.size(); j++) {
							if (vAnd[j]->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------

					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveRight(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vIs[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}

					accio = "PUSH";
					++numIs; // Player colisiona amb roca
					if (numIs == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numIs == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numIs == 1) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32 + 2, vIs[i1]->getposiciony() - 32));
				}
				else if (numIs == 2) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32 + 2, vIs[i1]->getposiciony() - 32));
					vIs[i2]->setPosition(glm::vec2(vIs[i2]->getposicionx() - 32 + 2, vIs[i2]->getposiciony() - 32));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaAnd) {
			int numAnd = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vAnd.size(); i++) {
				if (vAnd[i]->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					if (hihaIs) {
						for (int j = 0; j < vIs.size(); j++) { // Cartells is
							if (vIs[j]->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------
					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveRight(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vAnd[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}


					accio = "PUSH";
					++numAnd; // Player colisiona amb roca
					if (numAnd == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numAnd == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numAnd == 1) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32 + 2, vAnd[i1]->getposiciony() - 32));
				}
				else if (numAnd == 2) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32 + 2, vAnd[i1]->getposiciony() - 32));
					vAnd[i2]->setPosition(glm::vec2(vAnd[i2]->getposicionx() - 32 + 2, vAnd[i2]->getposiciony() - 32));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaBaba) {
			int i1;
			bool osbtacle = false;
				if (baba->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					if (hihaIs) {
						for (int j = 0; j < vIs.size(); j++) { // Cartells is
							if (vIs[j]->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaAnd) {
						for (int j = 0; j < vAnd.size(); j++) {
							if (vAnd[j]->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------
					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveRight(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									baba->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}

					accio = "PUSH";
					baba->setPosition(glm::vec2(baba->getposicionx() - 32 + 2, baba->getposiciony() - 32));
					string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaYou) {
			int i1;
			bool osbtacle = false;
			if (you->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell FALG
					if (flagC->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								you->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				you->setPosition(glm::vec2(you->getposicionx() - 32 + 2, you->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flagC->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								flagC->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				flagC->setPosition(glm::vec2(flagC->getposicionx() - 32 + 2, flagC->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWin) {
			int i1;
			bool osbtacle = false;
			if (win->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								win->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				win->setPosition(glm::vec2(win->getposicionx() - 32 + 2, win->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWall) {
			int i1;
			bool osbtacle = false;
			if (wall->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								wall->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}


				accio = "PUSH";
				wall->setPosition(glm::vec2(wall->getposicionx() - 32 + 2, wall->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaStop) {
			int i1;
			bool osbtacle = false;
			if (stop->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								stop->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				stop->setPosition(glm::vec2(stop->getposicionx() - 32 + 2, stop->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaLava) {
			int i1;
			bool osbtacle = false;
			if (lava->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Lava
					if (stop->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								lava->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				lava->setPosition(glm::vec2(lava->getposicionx() - 32 + 2, lava->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaDead) {
			int i1;
			bool osbtacle = false;
			if (die->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								die->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				die->setPosition(glm::vec2(die->getposicionx() - 32 + 2, die->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaRockCar) {
			int i1;
			bool osbtacle = false;
			if (rockCar->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								rockCar->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				rockCar->setPosition(glm::vec2(rockCar->getposicionx() - 32 + 2, rockCar->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaPush) {
			int i1;
			bool osbtacle = false;
			if (push->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Push 
					if (rockCar->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveRight(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								push->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				push->setPosition(glm::vec2(push->getposicionx() - 32 + 2, push->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}

		// ------------------------------------------------- OBJECTES ---------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flag->collisionMoveRight(glm::vec2(pos.x - 2, pos.y), size)) {

				if (stateFlag == flagWIN) {
					accio = "WIN";
					return;
				}
				else if (stateFlag == flagNOTHING) {
					accio = "PUSH";
				}

			}
		}
		if (hihaWallTile) { // --------------------------------- (Falta objectes + els estats)
			int numWall = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vWall.size(); i++) {
				if (vWall[i]->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

					if (stateWall == wallSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateWall == wallPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							 //Walls (Falta mirar els estats)
							for (int i22 = 0; i22 < vWall.size(); i22++) {
								if (vWall[i22]->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vWall[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
											return;
										}
									}
								}
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveRight(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
											return;
										}
									}
								}
							}
							
							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numWall; // Player colisiona amb roca
					if (numWall == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numWall == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numWall == 1) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32 + 2, vWall[i1]->getposiciony() - 32));
				}
				else if (numWall == 2) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32 + 2, vWall[i1]->getposiciony() - 32));
					vWall[i2]->setPosition(glm::vec2(vWall[i2]->getposicionx() - 32 + 2, vWall[i2]->getposiciony() - 32));
				}
			}
		}
		if (hihaRock)
		{ // --------------------------------- (Falta objectes + els estats)
			int numRocks = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vRocks.size(); i++) {
				if (vRocks[i]->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

					if (stateRock == rockSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateRock == rockWIN) {
						accio = "WIN";
						return;
					}
					else if (stateRock == rockPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							} 
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vRocks[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							for (int i2 = 0; i2 < vRocks.size(); i2++) {
								if (vRocks[i2]->collisionMoveRight(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
									osbtacle = true;
								}
							}
							accio = "PUSH";
						}
					} 
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numRocks; // Player colisiona amb roca
					if (numRocks == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					} 
					else if (numRocks == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			int xx;
			xx = numRocks;
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numRocks == 1) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32 + 2, vRocks[i1]->getposiciony() - 32));
				}
				else if (numRocks == 2) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32 + 2, vRocks[i1]->getposiciony() - 32));
					vRocks[i2]->setPosition(glm::vec2(vRocks[i2]->getposicionx() - 32 + 2, vRocks[i2]->getposiciony() - 32));
				}
			}
		}
		if (hihaLava) { // --------------------------------- (Falta objectes + els estats)
			int numLava = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vLavas.size(); i++) {
				if (vLavas[i]->collisionMoveRight(glm::vec2(pos.x, pos.y), size)) {

					if (stateLava == lavaSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateLava == lavaDIE) {
						accio = "DIE";
						return;
					}
					else if (stateLava == lavaPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveRightLimit(glm::vec2(pos.x + 32 + 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
								//fer una enum de moment. (Falta mirar els estats)
							for (int i2 = 0; i2 < vLavas.size(); i2++) {
								if (vLavas[i2]->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32 + 2), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}	
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveRight(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}

							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numLava; // Player colisiona amb roca
					if (numLava == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numLava == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numLava == 1) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32 + 2, vLavas[i1]->getposiciony() - 32));
				}
				else if (numLava == 2) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32 + 2, vLavas[i1]->getposiciony() - 32));
					vLavas[i2]->setPosition(glm::vec2(vLavas[i2]->getposicionx() - 32 + 2, vLavas[i2]->getposiciony() - 32));
				}
			}
		}	 

	}
}

bool TileMap::collisionMoveLeftLimit(const glm::ivec2& pos, const glm::ivec2& size) const
{

	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 1)
			return true;
	}

	return false;
}

void TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, string &accio)
{	

	accio = "PUSH";
	if (collisionMoveLeftLimit(glm::vec2(pos.x -2 , pos.y ), size)) {
		accio = "STOP";
		return;
	}
	else {
		// --------------------------------------------------- CARTELLS ------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaIs) {
			int numIs = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vIs.size(); i++) {
				if (vIs[i]->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {
					
					// Mira limits
					if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaAnd) { // Cartell AND
						for (int j = 0; j < vAnd.size(); j++) {
							if (vAnd[j]->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------

					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveLeft(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vIs[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}

					accio = "PUSH";
					++numIs; // Player colisiona amb roca
					if (numIs == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numIs == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numIs == 1) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32 - 2, vIs[i1]->getposiciony() - 32));
				}
				else if (numIs == 2) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32 - 2, vIs[i1]->getposiciony() - 32));
					vIs[i2]->setPosition(glm::vec2(vIs[i2]->getposicionx() - 32 - 2, vIs[i2]->getposiciony() - 32));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaAnd) {
			int numAnd = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vAnd.size(); i++) {
				if (vAnd[i]->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					if (hihaIs) {
						for (int j = 0; j < vIs.size(); j++) { // Cartells is
							if (vIs[j]->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------
					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveLeft(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vAnd[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}


					accio = "PUSH";
					++numAnd; // Player colisiona amb roca
					if (numAnd == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numAnd == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numAnd == 1) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32 - 2, vAnd[i1]->getposiciony() - 32));
				}
				else if (numAnd == 2) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32 - 2, vAnd[i1]->getposiciony() - 32));
					vAnd[i2]->setPosition(glm::vec2(vAnd[i2]->getposicionx() - 32 - 2, vAnd[i2]->getposiciony() - 32));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaBaba) {
			int i1;
			bool osbtacle = false;
				if (baba->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					if (hihaIs) {
						for (int j = 0; j < vIs.size(); j++) { // Cartells is
							if (vIs[j]->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaAnd) {
						for (int j = 0; j < vAnd.size(); j++) {
							if (vAnd[j]->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------
					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveLeft(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									baba->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}

					accio = "PUSH";
					baba->setPosition(glm::vec2(baba->getposicionx() - 32 - 2, baba->getposiciony() - 32));
					string acc;
					reReadStates(acc);
					if (acc == "WIN") {
						accio = "WIN";
						return;
					}

			}
		}
		if (hihaYou) {
			int i1;
			bool osbtacle = false;
			if (you->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell FALG
					if (flagC->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								you->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				you->setPosition(glm::vec2(you->getposicionx() - 32 - 2, you->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flagC->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								flagC->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				flagC->setPosition(glm::vec2(flagC->getposicionx() - 32 - 2, flagC->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWin) {
			int i1;
			bool osbtacle = false;
			if (win->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								win->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				win->setPosition(glm::vec2(win->getposicionx() - 32 - 2, win->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWall) {
			int i1;
			bool osbtacle = false;
			if (wall->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								wall->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}


				accio = "PUSH";
				wall->setPosition(glm::vec2(wall->getposicionx() - 32 - 2, wall->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaStop) {
			int i1;
			bool osbtacle = false;
			if (stop->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								stop->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				stop->setPosition(glm::vec2(stop->getposicionx() - 32 - 2, stop->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaLava) {
			int i1;
			bool osbtacle = false;
			if (lava->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Lava
					if (stop->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								lava->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				lava->setPosition(glm::vec2(lava->getposicionx() - 32 - 2, lava->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaDead) {
			int i1;
			bool osbtacle = false;
			if (die->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								die->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				die->setPosition(glm::vec2(die->getposicionx() - 32 - 2, die->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaRockCar) {
			int i1;
			bool osbtacle = false;
			if (rockCar->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								rockCar->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				rockCar->setPosition(glm::vec2(rockCar->getposicionx() - 32 - 2, rockCar->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaPush) {
			int i1;
			bool osbtacle = false;
			if (push->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Push 
					if (rockCar->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveLeft(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								push->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				push->setPosition(glm::vec2(push->getposicionx() - 32 - 2, push->getposiciony() - 32));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}

		// ------------------------------------------------- OBJECTES ---------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flag->collisionMoveLeft(glm::vec2(pos.x - 2, pos.y), size)) {

				if (stateFlag == flagWIN) {
					accio = "WIN";
					return;
				}
				else if (stateFlag == flagNOTHING) {
					accio = "PUSH";
				}

			}
		}
		if (hihaWallTile) { // --------------------------------- (Falta objectes + els estats)
			int numWall = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vWall.size(); i++) {
				if (vWall[i]->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

					if (stateWall == wallSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateWall == wallPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							 //Walls (Falta mirar els estats)
							for (int i22 = 0; i22 < vWall.size(); i22++) {
								if (vWall[i22]->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vWall[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveLeft(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							
							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numWall; // Player colisiona amb roca
					if (numWall == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numWall == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numWall == 1) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32 - 2, vWall[i1]->getposiciony() - 32));
				}
				else if (numWall == 2) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32 - 2, vWall[i1]->getposiciony() - 32));
					vWall[i2]->setPosition(glm::vec2(vWall[i2]->getposicionx() - 32 - 2, vWall[i2]->getposiciony() - 32));
				}
			}
		}
		if (hihaRock) { // --------------------------------- (Falta objectes + els estats)
			int numRocks = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vRocks.size(); i++) {
				if (vRocks[i]->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

					if (stateRock == rockSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateRock == rockWIN) {
						accio = "WIN";
						return;
					}
					else if (stateRock == rockPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							} 
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vRocks[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							for (int i2 = 0; i2 < vRocks.size(); i2++) {
								if (vRocks[i2]->collisionMoveLeft(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
									osbtacle = true;
								}
							}
							accio = "PUSH";
						}
					} 
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numRocks; // Player colisiona amb roca
					if (numRocks == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					} 
					else if (numRocks == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			int xx;
			xx = numRocks;
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numRocks == 1) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32 - 2, vRocks[i1]->getposiciony() - 32));
				}
				else if (numRocks == 2) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32 - 2, vRocks[i1]->getposiciony() - 32));
					vRocks[i2]->setPosition(glm::vec2(vRocks[i2]->getposicionx() - 32 - 2, vRocks[i2]->getposiciony() - 32));
				}
			}
		}
		if (hihaLava) { // --------------------------------- (Falta objectes + els estats)
			int numLava = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vLavas.size(); i++) {
				if (vLavas[i]->collisionMoveLeft(glm::vec2(pos.x, pos.y), size)) {

					if (stateLava == lavaSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateLava == lavaDIE) {
						accio = "DIE";
						return;
					}
					else if (stateLava == lavaPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveLeftLimit(glm::vec2(pos.x - 32 - 2, pos.y), size)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
								//fer una enum de moment. (Falta mirar els estats)
							for (int i2 = 0; i2 < vLavas.size(); i2++) {
								if (vLavas[i2]->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32 + 2), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}	
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveLeft(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}

							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numLava; // Player colisiona amb roca
					if (numLava == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numLava == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numLava == 1) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32 - 2, vLavas[i1]->getposiciony() - 32));
				}
				else if (numLava == 2) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32 - 2, vLavas[i1]->getposiciony() - 32));
					vLavas[i2]->setPosition(glm::vec2(vLavas[i2]->getposicionx() - 32 - 2, vLavas[i2]->getposiciony() - 32));
				}
			}
		}	 

	}
}

bool TileMap::collisionMoveUpLimit(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{

	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + 32 - 1) / tileSize;
	y = (pos.y) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 1)
		{
			return true;
		}
	}

	return false;
}


void TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, string& accio, int* posY)
{

	accio = "PUSH";
	if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 2), glm::ivec2(32, 32), posY)) {
		accio = "STOP";
		return;
	}
	else {
		// --------------------------------------------------- CARTELLS ------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaIs) {
			int numIs = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vIs.size(); i++) {
				if (vIs[i]->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaAnd) { // Cartell AND
						for (int j = 0; j < vAnd.size(); j++) {
							if (vAnd[j]->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------

					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveUp(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vIs[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}

					accio = "PUSH";
					++numIs; // Player colisiona amb roca
					if (numIs == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numIs == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numIs == 1) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32, vIs[i1]->getposiciony() - 32 - 2));
				}
				else if (numIs == 2) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32, vIs[i1]->getposiciony() - 32 - 2));
					vIs[i2]->setPosition(glm::vec2(vIs[i2]->getposicionx() - 32, vIs[i2]->getposiciony() - 32 - 2));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaAnd) {
			int numAnd = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vAnd.size(); i++) {
				if (vAnd[i]->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					if (hihaIs) {
						for (int j = 0; j < vIs.size(); j++) { // Cartells is
							if (vIs[j]->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------
					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveUp(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vAnd[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}


					accio = "PUSH";
					++numAnd; // Player colisiona amb roca
					if (numAnd == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numAnd == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numAnd == 1) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32, vAnd[i1]->getposiciony() - 32 - 2));
				}
				else if (numAnd == 2) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32, vAnd[i1]->getposiciony() - 32 - 2));
					vAnd[i2]->setPosition(glm::vec2(vAnd[i2]->getposicionx() - 32, vAnd[i2]->getposiciony() - 32 - 2));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaBaba) {
			int i1;
			bool osbtacle = false;
			if (baba->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaYou) { // Cartell You
					if (you->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell FALG
					if (flagC->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								baba->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				baba->setPosition(glm::vec2(baba->getposicionx() - 32, baba->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaYou) {
			int i1;
			bool osbtacle = false;
			if (you->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell FALG
					if (flagC->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								you->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				you->setPosition(glm::vec2(you->getposicionx() - 32, you->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flagC->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								flagC->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				flagC->setPosition(glm::vec2(flagC->getposicionx() - 32, flagC->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWin) {
			int i1;
			bool osbtacle = false;
			if (win->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								win->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				win->setPosition(glm::vec2(win->getposicionx() - 32, win->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWall) {
			int i1;
			bool osbtacle = false;
			if (wall->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								wall->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}


				accio = "PUSH";
				wall->setPosition(glm::vec2(wall->getposicionx() - 32, wall->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaStop) {
			int i1;
			bool osbtacle = false;
			if (stop->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								stop->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				stop->setPosition(glm::vec2(stop->getposicionx() - 32, stop->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaLava) {
			int i1;
			bool osbtacle = false;
			if (lava->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Lava
					if (stop->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								lava->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				lava->setPosition(glm::vec2(lava->getposicionx() - 32, lava->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaDead) {
			int i1;
			bool osbtacle = false;
			if (die->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								die->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				die->setPosition(glm::vec2(die->getposicionx() - 32, die->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaRockCar) {
			int i1;
			bool osbtacle = false;
			if (rockCar->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								rockCar->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				rockCar->setPosition(glm::vec2(rockCar->getposicionx() - 32, rockCar->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaPush) {
			int i1;
			bool osbtacle = false;
			if (push->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Push 
					if (rockCar->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveUp(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								push->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				push->setPosition(glm::vec2(push->getposicionx() - 32, push->getposiciony() - 32 - 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}

		// ------------------------------------------------- OBJECTES ---------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flag->collisionMoveUp(glm::vec2(pos.x, pos.y - 2), size)) {

				if (stateFlag == flagWIN) {
					accio = "WIN";
					return;
				}
				else if (stateFlag == flagNOTHING) {
					accio = "PUSH";
				}

			}
		}
		if (hihaWallTile) { // --------------------------------- (Falta objectes + els estats)
			int numWall = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vWall.size(); i++) {
				if (vWall[i]->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

					if (stateWall == wallSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateWall == wallPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							 //Walls (Falta mirar els estats)
							for (int i22 = 0; i22 < vWall.size(); i22++) {
								if (vWall[i22]->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vWall[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveUp(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}

							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numWall; // Player colisiona amb roca
					if (numWall == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numWall == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numWall == 1) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32, vWall[i1]->getposiciony() - 32 - 2));
				}
				else if (numWall == 2) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32, vWall[i1]->getposiciony() - 32 - 2));
					vWall[i2]->setPosition(glm::vec2(vWall[i2]->getposicionx() - 32, vWall[i2]->getposiciony() - 32 - 2));
				}
			}
		}
		if (hihaRock) { // --------------------------------- (Falta objectes + els estats)
			int numRocks = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vRocks.size(); i++) {
				if (vRocks[i]->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

					if (stateRock == rockSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateRock == rockWIN) {
						accio = "WIN";
						return;
					}
					else if (stateRock == rockPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vRocks[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							for (int i2 = 0; i2 < vRocks.size(); i2++) {
								if (vRocks[i2]->collisionMoveUp(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
									osbtacle = true;
								}
							}
							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numRocks; // Player colisiona amb roca
					if (numRocks == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numRocks == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			int xx;
			xx = numRocks;
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numRocks == 1) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32, vRocks[i1]->getposiciony() - 32 - 2));
				}
				else if (numRocks == 2) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32, vRocks[i1]->getposiciony() - 32 - 2));
					vRocks[i2]->setPosition(glm::vec2(vRocks[i2]->getposicionx() - 32, vRocks[i2]->getposiciony() - 32 - 2));
				}
			}
		}
		if (hihaLava) { // --------------------------------- (Falta objectes + els estats)
			int numLava = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vLavas.size(); i++) {
				if (vLavas[i]->collisionMoveUp(glm::vec2(pos.x, pos.y), size)) {

					if (stateLava == lavaSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateLava == lavaDIE) {
						accio = "DIE";
						return;
					}
					else if (stateLava == lavaPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveUpLimit(glm::vec2(pos.x, pos.y - 32 - 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							//fer una enum de moment. (Falta mirar els estats)
							for (int i2 = 0; i2 < vLavas.size(); i2++) {
								if (vLavas[i2]->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32 + 2), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}

							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numLava; // Player colisiona amb roca
					if (numLava == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numLava == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numLava == 1) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32, vLavas[i1]->getposiciony() - 32 - 2));
				}
				else if (numLava == 2) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32, vLavas[i1]->getposiciony() - 32 - 2));
					vLavas[i2]->setPosition(glm::vec2(vLavas[i2]->getposicionx() - 32, vLavas[i2]->getposiciony() - 32 - 2));
				}
			}
		}

	}
}



bool TileMap::collisionMoveDownLimit(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{

	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 1)
		{
			return true;
		}
	}
	return false;
}


void TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, string& accio, int* posY)
{

	accio = "PUSH";
	if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 2), glm::ivec2(32, 32), posY)) {
		accio = "STOP";
		return;
	}
	else {
		// --------------------------------------------------- CARTELLS ------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaIs) {
			int numIs = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vIs.size(); i++) {
				if (vIs[i]->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaAnd) { // Cartell AND
						for (int j = 0; j < vAnd.size(); j++) {
							if (vAnd[j]->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------

					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveDown(glm::vec2((vIs[i]->getposicionx() - 32), (vIs[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vIs[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}

					accio = "PUSH";
					++numIs; // Player colisiona amb roca
					if (numIs == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numIs == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numIs == 1) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32, vIs[i1]->getposiciony() - 32 + 2));
				}
				else if (numIs == 2) {
					vIs[i1]->setPosition(glm::vec2(vIs[i1]->getposicionx() - 32, vIs[i1]->getposiciony() - 32 + 2));
					vIs[i2]->setPosition(glm::vec2(vIs[i2]->getposicionx() - 32, vIs[i2]->getposiciony() - 32 + 2));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaAnd) {
			int numAnd = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vAnd.size(); i++) {
				if (vAnd[i]->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

					// Mira limits
					if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
						accio = "STOP";
						return;
					}
					// -------------------------------------CARTELLS--------------------------------------------
					if (hihaIs) {
						for (int j = 0; j < vIs.size(); j++) { // Cartells is
							if (vIs[j]->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								accio = "STOP";
								return;
							}
						}
					}
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaBaba) { //Cartell Baba
						if (baba->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaYou) { // Cartell You
						if (you->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaFlag) { // Cartell FALG
						if (flagC->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWin) { // Cartell Win
						if (win->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaWall) { // Cartell Wall
						if (wall->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaStop) { // Cartell Stop
						if (stop->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaLava) { // Cartell Lava
						if (lava->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaDead) { // Cartell Dead
						if (die->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaRockCar) { // Cartell Rock
						if (rockCar->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					if (hihaPush) { // Cartell Push 
						if (push->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
					// ---------------------------------------------OBJECTES----------------------------------------
					if (hihaWallTile) { //Walls (Falta mirar els estats)
						for (int i22 = 0; i22 < vWall.size(); i22++) {
							if (vWall[i22]->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateWall == wallSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateWall == wallNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaRock) {
						for (int i2 = 0; i2 < vRocks.size(); i2++) {
							if (vRocks[i2]->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateRock == rockPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateRock == rockNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}
					if (hihaLava) { // Lava
						for (int i2 = 0; i2 < vLavas.size(); i2++) {
							if (vLavas[i2]->collisionMoveDown(glm::vec2((vAnd[i]->getposicionx() - 32), (vAnd[i]->getposiciony() - 32)), size)) {
								if (stateLava == lavaDIE) {
									vAnd[i]->setPosition(glm::vec2(1000, 1000));
								}
								else if (stateLava == lavaSTOP) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaPUSH) {
									accio = "STOP";
									return;
								}
								else if (stateLava == lavaNOTHING) {
									accio = "PUSH";
								}
							}
						}
					}


					accio = "PUSH";
					++numAnd; // Player colisiona amb roca
					if (numAnd == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numAnd == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}

			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numAnd == 1) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32, vAnd[i1]->getposiciony() - 32 + 2));
				}
				else if (numAnd == 2) {
					vAnd[i1]->setPosition(glm::vec2(vAnd[i1]->getposicionx() - 32, vAnd[i1]->getposiciony() - 32 + 2));
					vAnd[i2]->setPosition(glm::vec2(vAnd[i2]->getposicionx() - 32, vAnd[i2]->getposiciony() - 32 + 2));
				}
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}
			}
		}
		if (hihaBaba) {
			int i1;
			bool osbtacle = false;
			if (baba->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaYou) { // Cartell You
					if (you->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell FALG
					if (flagC->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((baba->getposicionx() - 32), (baba->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								baba->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				baba->setPosition(glm::vec2(baba->getposicionx() - 32, baba->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaYou) {
			int i1;
			bool osbtacle = false;
			if (you->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell FALG
					if (flagC->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((you->getposicionx() - 32), (you->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								you->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				you->setPosition(glm::vec2(you->getposicionx() - 32, you->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flagC->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Win
					if (win->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((flagC->getposicionx() - 32), (flagC->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								flagC->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				flagC->setPosition(glm::vec2(flagC->getposicionx() - 32, flagC->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWin) {
			int i1;
			bool osbtacle = false;
			if (win->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Wall
					if (wall->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((win->getposicionx() - 32), (win->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								win->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				win->setPosition(glm::vec2(win->getposicionx() - 32, win->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaWall) {
			int i1;
			bool osbtacle = false;
			if (wall->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Stop
					if (stop->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((wall->getposicionx() - 32), (wall->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								wall->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}


				accio = "PUSH";
				wall->setPosition(glm::vec2(wall->getposicionx() - 32, wall->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaStop) {
			int i1;
			bool osbtacle = false;
			if (stop->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((stop->getposicionx() - 32), (stop->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								stop->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				stop->setPosition(glm::vec2(stop->getposicionx() - 32, stop->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaLava) {
			int i1;
			bool osbtacle = false;
			if (lava->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Lava
					if (stop->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Dead
					if (die->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((lava->getposicionx() - 32), (lava->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								lava->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				lava->setPosition(glm::vec2(lava->getposicionx() - 32, lava->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaDead) {
			int i1;
			bool osbtacle = false;
			if (die->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Rock
					if (rockCar->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((die->getposicionx() - 32), (die->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								die->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				die->setPosition(glm::vec2(die->getposicionx() - 32, die->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaRockCar) {
			int i1;
			bool osbtacle = false;
			if (rockCar->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaPush) { // Cartell Push 
					if (push->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((rockCar->getposicionx() - 32), (rockCar->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								rockCar->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				rockCar->setPosition(glm::vec2(rockCar->getposicionx() - 32, rockCar->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}
		if (hihaPush) {
			int i1;
			bool osbtacle = false;
			if (push->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

				// Mira limits
				if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
					accio = "STOP";
					return;
				}
				// -------------------------------------CARTELLS--------------------------------------------
				if (hihaIs) {
					for (int j = 0; j < vIs.size(); j++) { // Cartells is
						if (vIs[j]->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaAnd) {
					for (int j = 0; j < vAnd.size(); j++) {
						if (vAnd[j]->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							accio = "STOP";
							return;
						}
					}
				}
				if (hihaBaba) { // Cartell Baba
					if (baba->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaYou) { // Cartell FALG
					if (you->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaFlag) { // Cartell Win
					if (flagC->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWin) { // Cartell Wall
					if (win->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaWall) { // Cartell Stop
					if (wall->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaStop) { // Cartell Dead
					if (stop->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaLava) { // Cartell Lava
					if (lava->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaDead) { // Cartell Rock
					if (die->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				if (hihaRockCar) { // Cartell Push 
					if (rockCar->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
						accio = "STOP";
						return;
					}
				}
				// ---------------------------------------------OBJECTES----------------------------------------
				if (hihaWallTile) { //Walls (Falta mirar els estats)
					for (int i22 = 0; i22 < vWall.size(); i22++) {
						if (vWall[i22]->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateWall == wallSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateWall == wallNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaRock) {
					for (int i2 = 0; i2 < vRocks.size(); i2++) {
						if (vRocks[i2]->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateRock == rockPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateRock == rockNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}
				if (hihaLava) { // Lava
					for (int i2 = 0; i2 < vLavas.size(); i2++) {
						if (vLavas[i2]->collisionMoveDown(glm::vec2((push->getposicionx() - 32), (push->getposiciony() - 32)), size)) {
							if (stateLava == lavaDIE) {
								push->setPosition(glm::vec2(1000, 1000));
							}
							else if (stateLava == lavaSTOP) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaPUSH) {
								accio = "STOP";
								return;
							}
							else if (stateLava == lavaNOTHING) {
								accio = "PUSH";
							}
						}
					}
				}

				accio = "PUSH";
				push->setPosition(glm::vec2(push->getposicionx() - 32, push->getposiciony() - 32 + 2));
				string acc;
				reReadStates(acc);
				if (acc == "WIN") {
					accio = "WIN";
					return;
				}

			}
		}

		// ------------------------------------------------- OBJECTES ---------------------------------------------------- (Falta a tots el xoc amb objectes + els estats)
		if (hihaFlag) {
			int i1;
			bool osbtacle = false;
			if (flag->collisionMoveDown(glm::vec2(pos.x, pos.y - 2), size)) {

				if (stateFlag == flagWIN) {
					accio = "WIN";
					return;
				}
				else if (stateFlag == flagNOTHING) {
					accio = "PUSH";
				}

			}
		}
		if (hihaWallTile) { // --------------------------------- (Falta objectes + els estats)
			int numWall = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vWall.size(); i++) {
				if (vWall[i]->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

					if (stateWall == wallSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateWall == wallPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							 //Walls (Falta mirar els estats)
							for (int i22 = 0; i22 < vWall.size(); i22++) {
								if (vWall[i22]->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vWall[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveDown(glm::vec2((vWall[i]->getposicionx() - 32), (vWall[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}

							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numWall; // Player colisiona amb roca
					if (numWall == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numWall == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numWall == 1) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32, vWall[i1]->getposiciony() - 32 + 2));
				}
				else if (numWall == 2) {
					vWall[i1]->setPosition(glm::vec2(vWall[i1]->getposicionx() - 32, vWall[i1]->getposiciony() - 32 + 2));
					vWall[i2]->setPosition(glm::vec2(vWall[i2]->getposicionx() - 32, vWall[i2]->getposiciony() - 32 + 2));
				}
			}
		}
		if (hihaRock) { // --------------------------------- (Falta objectes + els estats)
			int numRocks = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vRocks.size(); i++) {
				if (vRocks[i]->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

					if (stateRock == rockSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateRock == rockWIN) {
						accio = "WIN";
						return;
					}
					else if (stateRock == rockPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							if (hihaLava) { // Lava
								//fer una enum de moment. (Falta mirar els estats)
								for (int i2 = 0; i2 < vLavas.size(); i2++) {
									if (vLavas[i2]->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
										if (stateLava == lavaDIE) {
											vRocks[i]->setPosition(glm::vec2(1000, 1000));
										}
										else if (stateLava == lavaSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateLava == lavaNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							for (int i2 = 0; i2 < vRocks.size(); i2++) {
								if (vRocks[i2]->collisionMoveDown(glm::vec2((vRocks[i]->getposicionx() - 32), (vRocks[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
									osbtacle = true;
								}
							}
							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numRocks; // Player colisiona amb roca
					if (numRocks == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numRocks == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			int xx;
			xx = numRocks;
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numRocks == 1) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32, vRocks[i1]->getposiciony() - 32 + 2));
				}
				else if (numRocks == 2) {
					vRocks[i1]->setPosition(glm::vec2(vRocks[i1]->getposicionx() - 32, vRocks[i1]->getposiciony() - 32 + 2));
					vRocks[i2]->setPosition(glm::vec2(vRocks[i2]->getposicionx() - 32, vRocks[i2]->getposiciony() - 32 + 2));
				}
			}
		}
		if (hihaLava) { // --------------------------------- (Falta objectes + els estats)
			int numLava = 0;
			int i1, i2;
			bool osbtacle = false;
			for (int i = 0; i < vLavas.size(); i++) {
				if (vLavas[i]->collisionMoveDown(glm::vec2(pos.x, pos.y), size)) {

					if (stateLava == lavaSTOP) {
						accio = "STOP";
						return;
					}
					else if (stateLava == lavaDIE) {
						accio = "DIE";
						return;
					}
					else if (stateLava == lavaPUSH) {
						//es podria fer un while  fer push de totes les pedres amb una variable o eso creo 
						//int contWhile = 32;
						//while (vRocks[i]->collisionMoveRightWithRocks(glm::vec2(pos.x + contWhile, pos.y), size, vRocks)){
						//	contWhile += 32;
						//}
						if (collisionMoveDownLimit(glm::vec2(pos.x, pos.y + 32 + 2), size, posY)) { // Miro si a la psosicio on es va a moure es colisio o no.
							accio = "STOP";
							return;
						}
						else {
							if (hihaIs) { // Cartells Is
								for (int i2 = 0; i2 < vIs.size(); i2++) {
									if (vIs[i2]->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaAnd) { // Cartells And
								for (int i2 = 0; i2 < vAnd.size(); i2++) {
									if (vAnd[i2]->collisionMoveUp(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										accio = "STOP";
										return;
									}
								}
							}
							if (hihaBaba) { //Cartell Baba
								if (baba->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaYou) { // Cartell You
								if (you->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaFlag) { // Cartell FALG
								if (flagC->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWin) { // Cartell Win
								if (win->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaWall) { // Cartell Wall
								if (wall->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaStop) { // Cartell Stop
								if (stop->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaLava) { // Cartell Lava
								if (lava->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaDead) { // Cartell Dead
								if (die->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRockCar) { // Cartell Rock
								if (rockCar->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaPush) { // Cartell Push 
								if (push->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							// Flag ingnorem


							// ARA MIREM ELS OBJECTES I ELS ESTATS D'ELLS (per a fer encara)
							if (hihaWallTile) { //Walls (Falta mirar els estats)
								for (int i22 = 0; i22 < vWall.size(); i22++) {
									if (vWall[i22]->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateWall == wallSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateWall == wallNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}
							//fer una enum de moment. (Falta mirar els estats)
							for (int i2 = 0; i2 < vLavas.size(); i2++) {
								if (vLavas[i2]->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32 + 2), (vLavas[i]->getposiciony() - 32)), size)) {
									accio = "STOP";
									return;
								}
							}
							if (hihaRock) {
								for (int i2 = 0; i2 < vRocks.size(); i2++) {
									if (vRocks[i2]->collisionMoveDown(glm::vec2((vLavas[i]->getposicionx() - 32), (vLavas[i]->getposiciony() - 32)), size)) {
										if (stateRock == rockPUSH) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockSTOP) {
											accio = "STOP";
											return;
										}
										else if (stateRock == rockNOTHING) {
											accio = "PUSH";
										}
									}
								}
							}

							accio = "PUSH";
						}
					}
					else {
						accio = "PUSH";
						osbtacle = true;
					}

					++numLava; // Player colisiona amb roca
					if (numLava == 1) { // si es la primera es mira i es gurada la i ;
						i1 = i;
					}
					else if (numLava == 2) { // si es la segona es mira i es gurada la i;
						i2 = i;
					}
				}
			}
			if (!osbtacle) { // si les roquen no han colisionat amb algo push sino stop
				if (numLava == 1) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32, vLavas[i1]->getposiciony() - 32 + 2));
				}
				else if (numLava == 2) {
					vLavas[i1]->setPosition(glm::vec2(vLavas[i1]->getposicionx() - 32, vLavas[i1]->getposiciony() - 32 + 2));
					vLavas[i2]->setPosition(glm::vec2(vLavas[i2]->getposicionx() - 32, vLavas[i2]->getposiciony() - 32 + 2));
				}
			}
		}

	}
}

void TileMap::readLvlStates(){
	stateFlag = flagNOTHING;
	stateWall = wallNOTHING;
	stateRock = rockNOTHING;
	stateLava = lavaNOTHING;
	for (int x = 0; x < mapSize.x; x++)
	{
		for (int y = 0; y < mapSize.y; ++y) {
			
			if (map[(x * mapSize.x) + y] == 6) { // Cartell is 
				// Miro horitzontalment
				if (y - 1 >= 0) {
					if (map[(x * mapSize.x) + (y - 1)] == 10) { // Miro el costt esquerra del is hi ha FLAG
						if (y + 1 < mapSize.y) {
							if (map[(x * mapSize.x) + (y + 1)] == 11) { // Miro el costt dret del is
								stateFlag = flagWIN;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 13) { // Miro el costt dret del is
								stateFlag = flagSTOP;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 17) { // Miro el costt dret del is
								stateFlag = flagPUSH;
							}
						}
					}
					if (map[(x * mapSize.x) + (y - 1)] == 12) {
						if (y + 1 < mapSize.y) {
							if (map[(x * mapSize.x) + (y + 1)] == 13) { // Miro el costt dret del is
								stateWall = wallSTOP;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 17) { // Miro el costt dret del is
								stateWall = wallPUSH;
							}
						}
					}
					if (map[(x * mapSize.x) + (y - 1)] == 16) {
						if (y + 1 < mapSize.y) {
							if (map[(x * mapSize.x) + (y + 1)] == 13) { // Miro el costt dret del is
								stateRock = rockSTOP;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 17) { // Miro el costt dret del is
								stateRock = rockPUSH;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 11) { // Miro el costt dret del is
								stateRock = rockWIN;
							}
						}
					}
					if (map[(x * mapSize.x) + (y - 1)] == 14) {
						if (y + 1 < mapSize.y) {
							if (map[(x * mapSize.x) + (y + 1)] == 13) { // Miro el costt dret del is
								stateLava = lavaSTOP;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 15) { // Miro el costt dret del is
								stateLava = lavaDIE;
							}
							if (map[(x * mapSize.x) + (y + 1)] == 17) { // Miro el costt dret del is
								stateLava = lavaPUSH;
							}
						}
					}
				}
				// MIro Verticalment
				if (x - 1 >= 0) {
					if (map[((x - 1) * mapSize.x) + y] == 10) {
						if (x + 1 < mapSize.x) {
							if (map[((x + 1) * mapSize.x) + y] == 11) { // Miro el costt dret del is
								stateFlag = flagWIN;
							}
							if (map[((x + 1) * mapSize.x) + y] == 13) { // Miro el costt dret del is
								stateFlag = flagSTOP;
							}
							if (map[((x + 1) * mapSize.x) + y] == 17) { // Miro el costt dret del is
								stateFlag = flagPUSH;
							}
						}
					}
					if (map[((x - 1) * mapSize.x) + y] == 12) {
						if (x + 1 < mapSize.x) {
							if (map[((x + 1) * mapSize.x) + y] == 13) { // Miro el costt dret del is
								stateWall = wallSTOP;
							}
							if (map[((x + 1) * mapSize.x) + y] == 17) { // Miro el costt dret del is
								stateWall = wallPUSH;
							}
						}
					}
					if (map[((x - 1) * mapSize.x) + y] == 16) {
						if (x + 1 < mapSize.x) {
							if (map[((x + 1) * mapSize.x) + y] == 13) { // Miro el costt dret del is
								stateRock = rockSTOP;
							}
							if (map[((x + 1) * mapSize.x) + y] == 17) { // Miro el costt dret del is
								stateRock = rockPUSH;
							}
						}
					}
					if (map[((x - 1) * mapSize.x) + y] == 14) {
						if (x + 1 < mapSize.x) {
							if (map[((x + 1) * mapSize.x) + y] == 13) { // Miro el costt dret del is
								stateLava = lavaSTOP;
							}
							if (map[((x + 1) * mapSize.x) + y] == 15) { // Miro el costt dret del is
								stateLava = lavaDIE;
							}
							if (map[((x + 1) * mapSize.x) + y] == 17) { // Miro el costt dret del is
								stateLava = lavaPUSH;
							}
						}
					}
				}
			}
			if (map[(x * mapSize.x) + y] == 7) { // Cartell and 

			}
		}
	}
}

void TileMap::reReadStates(string& accio) {

	stateFlagAnt = stateFlag;
	stateWallAnt = stateWall;
	stateRockAnt = stateRock;
	stateLavaAnt = stateLava;
	stateFlag = flagNOTHING;
	stateWall = wallNOTHING;
	stateRock = rockNOTHING;
	stateLava = lavaNOTHING;

	for (int i = 0; i < vIs.size(); i++) {
		// Primer mirem verticalment
		// statesFLAG
		if (hihaFlag) {
			if (flagC->collisionMoveUp(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaWin) {
					if (win->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) { // FLAG is WIN
						stateFlag = flagWIN;
					}
				}
				if (hihaStop) {
					if (stop->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// FLAG is STOP
						stateFlag = flagSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// FLAG is PUSH
						stateFlag = flagPUSH;
					}
				}
			}
		}
		// statesWALL
		if (hihaWall) {
			if (wall->collisionMoveUp(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaStop) {
					if (stop->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// WALL is STOP
						stateWall = wallSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// WALL is PUSH
						stateWall = wallPUSH;
					}
				}
			}
		}
		// statesROCK
		if (hihaRockCar) {
			if (rockCar->collisionMoveUp(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaStop) {
					if (stop->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is STOP
						stateRock = rockSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateRock = rockPUSH;
					}
				}
				if (hihaWin) {
					if (win->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateRock = rockWIN;
					}
				}
			}
		}
		// stateLAVA
		if (hihaLava) {
			if (lava->collisionMoveUp(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaStop) {
					if (stop->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is STOP
						stateLava = lavaSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateLava = lavaPUSH;
					}
				}
				if (hihaDead) {
					if (die->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateLava = lavaDIE;
					}
				}
			}
		}

		// Miro Horitzontalment
		if (hihaFlag) {
			if (flagC->collisionMoveLeft(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaWin) {
					if (win->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) { // FLAG is WIN
						stateFlag = flagWIN;
					}
				}
				if (hihaStop) {
					if (stop->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// FLAG is STOP
						stateFlag = flagSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// FLAG is PUSH
						stateFlag = flagPUSH;
					}
				}
				
			}
		}
		// statesWALL
		if (hihaWall) {
			if (wall->collisionMoveLeft(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaStop) {
					if (stop->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// WALL is STOP
						stateWall = wallSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// WALL is PUSH
						stateWall = wallPUSH;
					}
				}
			}
		}
		// statesROCK
		if (hihaRockCar) {
			if (rockCar->collisionMoveLeft(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaStop) {
					if (stop->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is STOP
						stateRock = rockSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateRock = rockPUSH;
					}
				}
				if (hihaWin) {
					if (win->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateRock = rockWIN;
					}
				}
			}
		}
		// stateLAVA
		if (hihaLava) {
			if (lava->collisionMoveLeft(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaStop) {
					if (stop->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is STOP
						stateLava = lavaSTOP;
					}
				}
				if (hihaPush) {
					if (push->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateLava = lavaPUSH;
					}
				}
				if (hihaDead) {
					if (die->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {// ROCK is PUSH
						stateLava = lavaDIE;
					}
				}
			}
		}
		if (hihaBaba) {
			if (baba->collisionMoveLeft(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaYou) {
					if (you->collisionMoveRight(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
						if (hihaAnd) {
							for (int j = 0; j < vAnd.size(); j++) {
								if (vAnd[i]->collisionMoveRight(glm::vec2(you->getposicionx() - 32, you->getposiciony() - 32), glm::vec2(32, 32))) {
									if (hihaWin) {
										if (win->collisionMoveRight(glm::vec2(vAnd[i]->getposicionx() - 32, vAnd[i]->getposiciony() - 32), glm::vec2(32, 32))) {
											accio = "WIN";
										}
									}
								}
							}
						}
					}
				}
			}

			if (baba->collisionMoveUp(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
				if (hihaYou) {
					if (you->collisionMoveDown(glm::vec2(vIs[i]->getposicionx() - 32, vIs[i]->getposiciony() - 32), glm::vec2(32, 32))) {
						if (hihaAnd) {
							for (int j = 0; j < vAnd.size(); j++) {
								if (vAnd[i]->collisionMoveDown(glm::vec2(you->getposicionx() - 32, you->getposiciony() - 32), glm::vec2(32, 32))) {
									if (hihaWin) {
										if (win->collisionMoveDown(glm::vec2(vAnd[i]->getposicionx() - 32, vAnd[i]->getposiciony() - 32), glm::vec2(32, 32))) {
											accio = "WIN";
											engineT->play2D("sounds/on.wav", false);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		

	 }
	 if (stateFlagAnt != stateFlag) {
		 engineT->play2D("sounds/on.wav", false);
		 stateFlagAnt = stateFlag;
	 }
	 if (stateWallAnt != stateWall) {
		 engineT->play2D("sounds/on.wav", false);
		 stateWallAnt = stateWall;
	 }
	 if (stateRockAnt != stateRock) {
		 engineT->play2D("sounds/on.wav", false);
		 stateRockAnt = stateRock;
	 }
	 if (stateLavaAnt != stateLava) {
		 engineT->play2D("sounds/on.wav", false);
		 stateLavaAnt = stateLava;
	 }
}

















