#include "bat.h"

Bat::Bat() {}

Bat::Bat(Graphics& graphics, Vector2 spawnPoint) :
	Enemy(graphics, "NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMove(false)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player& player) {
	this->_direction = player.getX() > this->_x ? Direction::RIGHT : Direction::LEFT;
	this->playAnimation(this->_direction == Direction::RIGHT ? "FlyRight" : "FlyLeft");

	//moves bat up or down
	this->_y += this->_shouldMove ? -.02 : .02;
	if (this->_y > (this->_startingY + 25) || this->_y < this->_startingY - 25) {
		this->_shouldMove = !this->_shouldMove;
	}

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics& graphics) {
	Enemy::draw(graphics);
}

void Bat::touchPlayer(Player* player) {
	player->gainHealth(-1);
}

void Bat::animationDone(string currentAnimation) {}

void Bat::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}