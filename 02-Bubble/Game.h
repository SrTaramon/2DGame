#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Menu.h"
#include "Credits.h"
#include "Instruccions.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void changeLvl(int lvlId);
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:

	int currentlvl;
	Scene scene;
	Scene1 scene1;
	Scene2 scene2;
	Scene3 scene3;
	Scene4 scene4;
	Instruccions ins;
	Credits credit;
	Menu menu;
	bool insCreat, credCreat, menuCreat, playCreat;
	bool bPlay;                       // Continue to play game?
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

	enum
	{
		PLAYING,
		MENU,
		CREDITS,
		INSTRUCTIONS
	};
	int state;
};


#endif // _GAME_INCLUDE


