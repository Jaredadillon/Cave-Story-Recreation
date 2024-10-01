#pragma once
#ifndef DOOR_H
#define DOOR_H

#include "rectangle.h"
#include <string>

using namespace std;

class Door : public Rectangle {
public:
	//default constructor
	Door() {}
	//custom constructor
	Door(Rectangle r, string destination) :
		Rectangle(r.getLeft()* globals::SPRITE_SCALE, r.getTop()* globals::SPRITE_SCALE, r.getWidth()* globals::SPRITE_SCALE, r.getHeight()* globals::SPRITE_SCALE), _destination(destination)
	{}

	//inline functions to get door rectangle and set destination
	const inline Rectangle getRectangle() const {
		return this->getRect();
	}
	const inline string getDestination() const {
		return this->_destination;
	}

private:
	string _destination;
};


#endif // !DOOR_H