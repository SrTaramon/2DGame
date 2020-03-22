#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STAND_UP, STAND_DOWN, MOVE_UP, MOVE_DOWN
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/Player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram); //tamany quad on aplico imag
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.25f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(STAND_UP, 8);
		sprite->addKeyframe(STAND_UP, glm::vec2(0.f, 0.75f));

		sprite->setAnimationSpeed(STAND_DOWN, 8);
		sprite->addKeyframe(STAND_DOWN, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.5f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.5f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

		sprite->setAnimationSpeed(MOVE_UP, 8);
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.25f, 0.75f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.5f, 0.75f));
		sprite->addKeyframe(MOVE_UP, glm::vec2(0.75f, 0.75f));

		sprite->setAnimationSpeed(MOVE_DOWN, 8);
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.75f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
		for (int i = 0; i < rocks.size(); ++i){
			if (rocks[i]->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) {
				rocks[i]->setPosition(glm::ivec2((rocks[i]->getposicionx() - 32) - 2, (rocks[i]->getposiciony() - 16)));
			}
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		} 
		for (int i = 0; i < rocks.size(); ++i){
			if (rocks[i]->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
				rocks[i]->setPosition(glm::ivec2((rocks[i]->getposicionx() - 32) + 2, (rocks[i]->getposiciony() - 16)));
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (sprite->animation() != MOVE_UP)
			sprite->changeAnimation(MOVE_UP);
		posPlayer.y -= 2;
		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			posPlayer.y += 2;
			sprite->changeAnimation(STAND_UP);
		}
		for (int i = 0; i < rocks.size(); ++i){
			if (rocks[i]->collisionMoveUp(posPlayer, glm::ivec2(32, 32))) {
				rocks[i]->setPosition(glm::ivec2((rocks[i]->getposicionx() - 32), (rocks[i]->getposiciony() - 16) - 2));
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
		posPlayer.y += 2;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			//posPlayer.y -= 2;
			sprite->changeAnimation(STAND_DOWN);
		}
		for (int i = 0; i < rocks.size(); ++i){
			if (rocks[i]->collisionMoveDown(posPlayer, glm::ivec2(32, 32))) {
				rocks[i]->setPosition(glm::ivec2((rocks[i]->getposicionx() - 32), (rocks[i]->getposiciony() - 16) + 2));
			}
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
		else if (sprite->animation() == MOVE_UP)
			sprite->changeAnimation(STAND_UP);
		else if (sprite->animation() == MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);
	}
	
	/*if(bJumping)
	{
		if (sprite->animation() != MOVE_UP) {
			sprite->changeAnimation(MOVE_UP);
		}
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		if (sprite->animation() != MOVE_DOWN) {
			sprite->changeAnimation(MOVE_DOWN);
		}
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}*/
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setRocks(Rocks* Rocks)
{
	rocks.push_back(Rocks);
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




