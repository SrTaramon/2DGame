#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Menu.h"


void Game::init()
{
	state = PLAYING;
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	menu.init();
	credits.init();
}

bool Game::update(int deltaTime)
{
	bool i = true;
	switch (state)
	{
	case PLAYING:
		scene.update(deltaTime);
		break;
	case MENU:
		menu.update(deltaTime);
		break;
	case CREDITS:
		//credits.update(deltaTime);
		break;
	default:
		break;
	}

	return bPlay;
}

void Game::render()
{
	bool i = true;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (state)
	{
	case MENU:
		menu.render();
		break;
	case PLAYING:
		scene.render();
		i = true;
		break;
	case CREDITS:
		if (i) {
			credits.render();
			i = false;
		}
	default:
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		if (state != MENU) {
			bPlay = false;
		}
		else {
			state = PLAYING;
		}
	}
	else if (key == 109) { //M
		state = MENU;
	}
	else if (key == 110) {
		state = PLAYING;
	}
	else if (key == 99) {
		state = CREDITS;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





