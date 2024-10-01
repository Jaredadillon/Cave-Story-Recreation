#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

class Rectangle {
public:
	//default constructor
	Rectangle() {}

	//custom constructor
	Rectangle(int x, int y, int width, int height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height)
	{}


	const int getCenterX() const {
		return this->_x + this->_width / 2;
	}

	const int getCenterY() const {
		return this->_y + this->_height / 2;
	}

	const int getLeft() const {
		return this->_x;
	}

	const int getRight() const {
		return this->_x + this->_width;
	}

	const int getTop() const {
		return this->_y;
	}

	const int getBottom() const {
		return this->_y + this->_height;
	}

	const int getWidth() {
		return this->_width;
	}

	const int getHeight() {
		return this->_height;
	}

	const int getSide(const sides::Side side) const {
		return
			side == sides::Side::LEFT ? this->getLeft() :
			side == sides::Side::RIGHT ? this->getRight() :
			side == sides::Side::TOP ? this->getTop() :
			side == sides::Side::BOTTOM ? this->getBottom() :
			side == sides::Side::NONE;
	}

	//determine if a collision is happening
	const bool collidesWith(const Rectangle& other) const {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop();
	}

	//is the rectangle a valid rectangle?
	const bool isValidRectangle() const {
		return(this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	//inline function to return the current rectangle in the vector
	const inline Rectangle getRect() const {
		return *this;
	}

private:

	int _x;
	int _y;
	int _width;
	int _height;

};



#endif // !RECTANGLE_H