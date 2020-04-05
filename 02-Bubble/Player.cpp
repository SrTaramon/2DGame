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


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int lvl)
{
	bJumping = false;
	lvlId = lvl;
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

		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		string accio;
		map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), accio);
		int z;
		if (accio == "PUSH") {
			posPlayer.x -= 2;
		}
		if (accio == "STOP")
		{
			sprite->changeAnimation(STAND_LEFT);
		}
		if (accio == "WIN") {
			lvlId += 1;
			Game::instance().changeLvl(lvlId);
		}
		if (accio == "DIE") {
			Game::instance().changeLvl(lvlId);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{

		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		
		string accio;
		map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), accio);
		int z;
		if (accio == "PUSH") {
			posPlayer.x += 2;
		}
		if (accio == "STOP")
		{
			sprite->changeAnimation(STAND_RIGHT);
		} 
		if (accio == "WIN") {
			lvlId += 1;
			Game::instance().changeLvl(lvlId);
		}
		if (accio == "DIE") {
			Game::instance().changeLvl(lvlId);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		if (sprite->animation() != MOVE_UP)
			sprite->changeAnimation(MOVE_UP);

		string accio;
		map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), accio, &posPlayer.y);
		int z;
		if (accio == "PUSH") {
			posPlayer.y -= 2;
		}
		if (accio == "STOP")
		{
			sprite->changeAnimation(STAND_UP);
		}
		if (accio == "WIN") {
			lvlId += 1;
			Game::instance().changeLvl(lvlId);
		}
		if (accio == "DIE") {
			Game::instance().changeLvl(lvlId);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		if (sprite->animation() != MOVE_DOWN)
			sprite->changeAnimation(MOVE_DOWN);

		string accio;
		map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), accio, &posPlayer.y);
		int z;
		if (accio == "PUSH") {
			posPlayer.y += 2;
		}
		if (accio == "STOP")
		{
			sprite->changeAnimation(STAND_DOWN);
		}
		if (accio == "WIN") {
			lvlId += 1;
			Game::instance().changeLvl(lvlId);
		}
		if (accio == "DIE") {
			Game::instance().changeLvl(lvlId);
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
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setLvl(int id) {
	lvlId = id;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




