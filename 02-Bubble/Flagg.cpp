#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Flagg.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND
};


void Flagg::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/Flag_SpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.33, 1), &spritesheet, &shaderProgram); //tamany quad on aplico imag
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(STAND, glm::vec2(0.33f, 0.0f));
	sprite->addKeyframe(STAND, glm::vec2(0.66f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Flagg::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (sprite->animation() != STAND)
		sprite->changeAnimation(STAND);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Flagg::render()
{
	sprite->render();
}

void Flagg::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Flagg::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




