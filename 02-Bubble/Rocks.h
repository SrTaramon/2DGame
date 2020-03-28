#ifndef _ROCKS_INCLUDE
#define _ROCKS_INCLUDE

#include <vector>
#include "Sprite.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Rocks
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionWallMoveRight(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock);
	bool collisionWallMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock);
	bool collisionWallMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock);
	bool collisionWallMoveDown(const glm::ivec2& pos, const glm::ivec2& size, const vector<Rocks*>& VRock);
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


#endif // _ROCKS_INCLUDE


