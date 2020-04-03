#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Is.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4



void Is::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/is.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.9), &spritesheet, &shaderProgram); //tamany quad on aplico imag
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

}

void Is::update(int deltaTime)
{
	sprite->update(deltaTime);

	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Is::render()
{
	sprite->render();
}


void Is::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


bool Is::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) {
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

bool Is::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) {
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

bool Is::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) {
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

bool Is::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size) {

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

	//----------------------------
}




