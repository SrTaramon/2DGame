#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "You.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void You::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/you.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.9), &spritesheet, &shaderProgram); //tamany quad on aplico imag
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

}

void You::update(int deltaTime)
{
	sprite->update(deltaTime);

}

void You::render()
{
	sprite->render();
}


void You::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


bool You::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) {
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

bool You::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) {
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

bool You::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 16;
	int x2 = sprite->getposicionx() - 32 + 32;
	int px = pos.x;
	int py = pos.y;
	int px2 = pos.x + 32;

	if (((py) > y & py < y + size.y)) {
		if (x < px) {
			if (px < x2) {
				return true;
			}
			else return false;
		}
		else if (px < x) {
			if (x < px2) {
				return true;
			}
			else return false;
		}
		else return true;
	}


	return false;
}

bool You::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 16;
	int x2 = sprite->getposicionx() - 32 + 32;
	int px = pos.x;
	int py = pos.y + 32;
	int px2 = pos.x + 32;

	if (((py) > y & py < y + size.y)) {
		if (x < px) {
			if (px < x2) {
				return true;
			}
			else return false;
		}
		else if (px < x) {
			if (x < px2) {
				return true;
			}
			else return false;
		}
		else return true;
	}


	return false;
}




