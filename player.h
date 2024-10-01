#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "animatedSprite.h"
#include "slope.h"
#include "door.h"
#include "level.h"
#include "enemy.h"

using namespace std;

class Player : public AnimatedSprite {

public:
	//default constructor
	Player();

	//custom constructor
	Player(Graphics& graphics, Vector2 spawnPoint);

	//draw method for the player class
	void draw(Graphics& graphics);

	//update method for the player class
	void update(float elapsedTime);

	//move the player left -delta x
	void moveLeft();
	//move the player right +delta x
	void moveRight();
	//stop player movement
	void stopMoving();
	//make player look up
	void lookUp();
	//make player stop looking up
	void stopLookingUp();
	//make player look down
	void lookDown();
	//make player stop looking down
	void stopLookingDown();
	//make Quote jump
	void jump();

	//override the abstract functions in animatedSprite
	virtual void animationDone(string currentAnimation);
	virtual void setupAnimations();
	void handleTileCollisions(vector<Rectangle> &others);
	void handleSlopeCollisions(vector<Slope> &others);
	void handleDoorCollisions(vector<Door>& others, Level& level, Graphics& graphics);
	void handleEnemyCollisions(vector<Enemy*>& others);

	const float getX() const;
	const float getY() const;

	//getter function to get max health
	const inline int getMaxHealth() {
		return this->_maxHealth;
	}

	//getter function to get current health
	const inline int getCurrentHealth() {
		return this->_currentHealth;
	}

	void gainHealth(int amount);

private:
	float _dx, _dy;
	Direction _facing;
	bool _grounded;
	bool _lookingUp = false;
	bool _lookingDown = false;
	int _maxHealth;
	int _currentHealth;
};

#endif // !PLAYER_H
