#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Push.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void Push::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/push.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1), &spritesheet, &shaderProgram); //tamany quad on aplico imag
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

}

void Push::update(int deltaTime)
{
	sprite->update(deltaTime);

	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Push::render()
{
	sprite->render();
}


void Push::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


bool Push::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 16;
	int y2 = sprite->getposiciony() - 16 + 32;

	int px = pos.x + 32;
	int py = pos.y;
	int py2 = pos.y + 32;


	if (((px) > x& px < x + size.x)) {

		if (py > y) {
			if (py < y2) {
				return true;
			}
			else return false;
		}
		else if (py < y) {
			if (py2 > y) {
				return true;
			}
			else false;
		}
		else return true;

	}



	return false;
}

bool Push::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 16;
	int y2 = sprite->getposiciony() - 16 + 32;

	int px = pos.x;
	int py = pos.y;
	int py2 = pos.y + 32;


	if (((px) > x& px < x + size.x)) {

		if (py > y) {
			if (py < y2) {
				return true;
			}
			else return false;
		}
		else if (py < y) {
			if (py2 > y) {
				return true;
			}
			else false;
		}
		else return true;

	}



	return false;
}

bool Push::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 16;
	int x2 = sprite->getposicionx() - 32 + 32;
	int px = pos.x;
	int py = pos.y;
	int px2 = pos.x + 32;

	if (((py) > y& py < y + size.y)) {
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

bool Push::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size) {
	int x, y;
	x = sprite->getposicionx() - 32;
	y = sprite->getposiciony() - 16;
	int x2 = sprite->getposicionx() - 32 + 32;
	int px = pos.x;
	int py = pos.y + 32;
	int px2 = pos.x + 32;

	if (((py) > y& py < y + size.y)) {
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




