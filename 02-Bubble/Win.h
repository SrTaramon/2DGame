#ifndef _WIN_INCLUDE
#define _WIN_INCLUDE


#include "Sprite.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Win
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size);
	int getposicionx() const { return sprite->getposicionx(); };
	int getposiciony() const { return sprite->getposiciony(); };


private:
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;

};


#endif 


