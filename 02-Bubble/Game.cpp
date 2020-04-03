#include <GL/glew.h>
#include <GL/glut.h>


#include "Game.h"



void Game::init()
{
	state = MENU;
	bPlay = true;
	insCreat = false;
	credCreat = false;
	menuCreat = true;
	playCreat = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	menu.init();
	credit.init();
	ins.init();
}

bool Game::update(int deltaTime)
{
	switch (state)
	{
	case PLAYING:
		scene.update(deltaTime);
		break;
	case MENU:
		menu.update(deltaTime);
		break;
	case INSTRUCTIONS:
		ins.update(deltaTime);
		break;
	case CREDITS:
		credit.update(deltaTime);
		break;
	default:
		break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (state)
	{
	case MENU:
		menu.render();
		break;
	case PLAYING:
		scene.render();
		break;
	case INSTRUCTIONS:
		ins.render();
		break;
	case CREDITS:
		credit.render();
		break;
	default:
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		if (state != MENU && state != CREDITS && state != INSTRUCTIONS) {
			bPlay = false;
		}
		else {
			state = PLAYING;
		}
		if (insCreat) {
			insCreat = false;
			ins.sounds(0);
		}
		if (credCreat) {
			credCreat = false;
			credit.sounds(0);
		}
		if (menuCreat){
			menuCreat = false;
			menu.sounds(0);
		}
		playCreat = true;
		scene.sounds(2);
	}
	else if (key == 109) { //M
		if (state != MENU) {
			state = MENU;
			if (insCreat) {
				insCreat = false;
				ins.sounds(0);
			}
			if (credCreat) {
				credCreat = false;
				credit.sounds(0);
			}
			if (playCreat) {
				playCreat = false;
				scene.sounds(0);
			}
			menu.sounds(2);
			menuCreat = true;
		}
	}
	else if (key == 99) { // C
		if (state != CREDITS) {
			state = CREDITS;
			if (insCreat) {
				insCreat = false;
				ins.sounds(0);
			}
			if (menuCreat) {
				menuCreat = false;
				menu.sounds(0);
			}
			if (playCreat) {
				playCreat = false;
				scene.sounds(0);
			}
			credit.sounds(2);
			credCreat = true;
		}
	}
	else if (key == 105) {
		if (state != INSTRUCTIONS) {
			state = INSTRUCTIONS;
			if (credCreat) {
				credCreat = false;
				credit.sounds(0);
			}
			if (menuCreat) {
				menuCreat = false;
				menu.sounds(0);
			}
			if (playCreat) {
				playCreat = false;
				scene.sounds(0);
			}
			ins.sounds(2);
			insCreat = true;
		}
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





