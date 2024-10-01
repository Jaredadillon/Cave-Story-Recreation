#include "enemy.h"

//default constructor
Enemy::Enemy() {}

//custom constructor
Enemy::Enemy(Graphics& graphics, string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(Direction::LEFT),
	_maxHealth(0),
	_currentHealth(0)
{}

void Enemy::update(int elapsedTime, Player& player) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}