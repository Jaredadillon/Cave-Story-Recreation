#pragma once
#ifndef HUD_H
#define HUD_H

#include "sprite.h"
#include "player.h"

class HUD {
public:
	//default constructor
	HUD();
	//custom constructor
	HUD(Graphics& graphics, Player& player);
	//update function for the HUD class
	void update(int elapsedTime, Player &player);
	//draw function for the HUD class
	void draw(Graphics& graphics);

private:
	Player _player;
	
	//health HUD sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	//EXP HUD sprites
	Sprite _lvWord;
	Sprite _lvNumber;
	Sprite _expBar;

	//weapon HUD sprites
	Sprite _slash;
	Sprite _dashes;
};

#endif // !HUD_H
