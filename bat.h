#pragma once
#ifndef BAT_H
#define BAT_H

#include "enemy.h"

class Bat : public Enemy {
public:
	Bat();
	Bat(Graphics& graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player& player);
	void draw(Graphics& graphics);
	void touchPlayer(Player* player); //handles player health indirectly
	void animationDone(string currentAnimation);
	void setupAnimations();

private:
	float _startingX;
	float _startingY;
	bool _shouldMove;
};


#endif // !BAT_H