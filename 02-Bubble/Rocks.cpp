#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Rocks.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void Rocks::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/rock-pixel.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.9), &spritesheet, &shaderProgram); //tamany quad on aplico imag
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

}

void Rocks::update(int deltaTime)
{
	sprite->update(deltaTime);

	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Rocks::render()
{
	sprite->render();
}


void Rocks::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


/*bool Rocks::collisionWallMoveRight(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks *>& VRock) {
	
	//bool Mapcollision = map->collisionMoveRight(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32));
	bool RockCollision = false;

		for (int i = 0; i < VRock.size(); i++) {
			if (VRock[i]->collisionMoveRight(glm::ivec2(pos.x , pos.y), glm::ivec2(32, 32))) {
				int x, y, y_2;
				x = VRock[i]->getposicionx() - 32;
				y = VRock[i]->getposiciony() -32;
				y_2 = y + 32;
				for (int j = 0; j < VRock.size(); j++) {
					if (i != j) {
						int x2, y2, y2_2;
						x2 = VRock[j]->getposicionx() - 32;
						y2 = VRock[j]->getposiciony() - 32;
						y2_2 = y2 + 32;
						if ((x + 32)  == x2) {
							if (y_2 <= y2) RockCollision = false;
							else if (y >= y2_2) RockCollision = false;
							else  return true;
						}
				
					}
				}
				if (map->collisionMoveRight(glm::ivec2(pos.x + 32, pos.y), glm::ivec2(32, 32))) {
					return true;
				}
			}
		}

	return (RockCollision);
}

bool Rocks::collisionWallMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock) {

	//bool Mapcollision = map->collisionMoveRight(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32));
	bool RockCollision = false;

	for (int i = 0; i < VRock.size(); i++) {
		if (VRock[i]->collisionMoveLeft(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32))) {
			int x, y, y_2;
			x = VRock[i]->getposicionx() - 32;
			y = VRock[i]->getposiciony() - 32;
			y_2 = y + 32;
			for (int j = 0; j < VRock.size(); j++) {
				if (i != j) {
					int x2, y2, y2_2;
					x2 = VRock[j]->getposicionx() - 32;
					y2 = VRock[j]->getposiciony() - 32;
					y2_2 = y2 + 32;
					if ((x - 32) == x2) {
						if (y_2 <= y2) RockCollision = false;
						else if (y >= y2_2) RockCollision = false;
						else  return true;
					}

				}
			}
			if (map->collisionMoveLeft(glm::ivec2(pos.x - 32, pos.y), glm::ivec2(32, 32))) {
				return true;
			}
		}
	}

	return (RockCollision);
}

bool Rocks::collisionWallMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock) {

	//bool Mapcollision = map->collisionMoveRight(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32));
	bool RockCollision = false;

	for (int i = 0; i < VRock.size(); i++) {
		if (VRock[i]->collisionMoveUp(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32))) {
			int x, y, x_2;
			x = VRock[i]->getposicionx() - 32;
			x_2 = x + 32;
			y = VRock[i]->getposiciony() - 32;

			for (int j = 0; j < VRock.size(); j++) {
				if (i != j) {
					int x2, y2, x2_2;
					x2 = VRock[j]->getposicionx() - 32;
					x2_2 = x2 + 32;
					y2 = VRock[j]->getposiciony() - 32;
					if ((y - 32) == y2) {
						if (x_2 <= x2) RockCollision = false;
						else if (x >= x2_2) RockCollision = false;
						else  return true;
					}

				}
			}
			if (map->collisionMoveUp(glm::ivec2(pos.x , pos.y - 32), glm::ivec2(32, 32), (&posPlayer.y))) {
				return true;
			}
		}
	}

	return (RockCollision);
}

bool Rocks::collisionWallMoveDown(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock) {

	//bool Mapcollision = map->collisionMoveRight(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32));
	bool RockCollision = false;

	for (int i = 0; i < VRock.size(); i++) {
		if (VRock[i]->collisionMoveDown(glm::ivec2(pos.x, pos.y), glm::ivec2(32, 32))) {
			int x, y, x_2;
			x = VRock[i]->getposicionx() - 32;
			x_2 = x + 32;
			y = VRock[i]->getposiciony() - 32;

			for (int j = 0; j < VRock.size(); j++) {
				if (i != j) {
					int x2, y2, x2_2;
					x2 = VRock[j]->getposicionx() - 32;
					x2_2 = x2 + 32;
					y2 = VRock[j]->getposiciony() - 32;
					if ((y + 32) == y2) {
						if (x_2 <= x2) RockCollision = false;
						else if (x >= x2_2) RockCollision = false;
						else  return true;
					}

				}
			}
			if (map->collisionMoveDown(glm::ivec2(pos.x, pos.y + 32), glm::ivec2(32, 32), &posPlayer.y)) {
				return true;
			}
		}
	}

	return (RockCollision);
}*/


bool Rocks::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 32;
	int y2 = sprite->getposiciony() - 32 + 32;

	int px = (pos.x) + 32;
	int py = pos.y;
	int py2 = pos.y + 32;
	
	if ((px) == x) {
		if (py >= y2) return false;
		else if (py2 <= y) return false;
		else  return true;
	}
	else return false;
}

bool Rocks::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 32;
	int y2 = sprite->getposiciony() - 32 + 32;

	int px = pos.x - 32;
	int py = pos.y;
	int py2 = pos.y + 32;


	if ((px) == x) {
		if (py >= y2) return false;
		else if (py2 <= y) return false;
		else  return true;
	}
	else return false;
}

bool Rocks::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 32 + 32;
	int x2 = x + 32;
	int px = pos.x;
	int px2 = pos.x + 32;
	int py = pos.y;



	if ((py) == y) {
		if (px2 <= x) return false;
		else if (px >= x2) return false;
		else  return true;
	}
	else return false;
}

bool Rocks::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 32;
	int x2 = x + 32;
	int px = pos.x;
	int px2 = pos.x + 32;
	int py = pos.y + 32;



	if ((py) == y) {
		if (px2 <= x) return false;
		else if (px >= x2) return false;
		else  return true;
	}
	else return false;
}




