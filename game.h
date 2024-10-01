#pragma once
#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "input.h"
#include "hud.h"
#include <SDL.h>
#include <algorithm>

using namespace std;

//Game class holds all information about the game loop
class Game {
public:
	Game();
	~Game();

private:
	void gameLoop();
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	Player _player;
	Level _level;
	HUD _hud;
	Graphics _graphics;
};


#endif