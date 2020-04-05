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
	for (int i = 0; i < lvls.size(); ++i) {
		Scene* s = new Scene();
		lvls.push_back(s);
		lvls[i]->init(i);
	}
	currentlvl = 0;
	menu.init();
	credit.init();
	ins.init();
}

bool Game::update(int deltaTime)
{
	switch (state)
	{
	case PLAYING:
		lvls[currentlvl]->update(deltaTime);
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
		lvls[currentlvl]->render();
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

void Game::changeLvl(int lvlId) {
	currentlvl = lvlId;
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
		lvls[currentlvl]->sounds(2);
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
				lvls[currentlvl]->sounds(0);
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
				lvls[currentlvl]->sounds(0);
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
				lvls[currentlvl]->sounds(0);
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





