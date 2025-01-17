#pragma once
#ifndef SLOPE_H
#define SLOPE_H

#include "rectangle.h"
#include <algorithm>

class Slope {
public:
	//default constructor
	Slope() {}

	//custom constructor
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),
		_p2(p2) {
		if (this->_p2.x - this->_p1.x != 0) {
			this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
		}
	}

	//getter function to get Slope value
	const inline float getSlope() const {
		return this->_slope;
	}

	//function to figure out where on the polyline the collision is happening
	const bool collidesWith(const Rectangle& other) const {
		return
			(other.getRight() >= this->_p2.x &&
				other.getLeft() <= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y) ||

				(other.getRight() >= this->_p1.x &&
					other.getLeft() <= this->_p2.x &&
					other.getTop() <= this->_p1.y &&
					other.getBottom() >= this->_p2.y) ||

					(other.getLeft() <= this->_p1.x &&
						other.getRight() >= this->_p2.x &&
						other.getTop() <= this->_p1.y &&
						other.getBottom() >= this->_p2.y) ||

						(other.getLeft() <= this->_p2.x &&
							other.getRight() >= this->_p1.x &&
							other.getTop() <= this->_p2.y &&
							other.getBottom() >= this->_p1.y);
	}

	//getter function to get point1
	const inline Vector2 getP1() const {
		return this->_p1;
	}

	//getter function to get point2
	const inline Vector2 getP2() const {
		return this->_p2;
	}

private:
	Vector2 _p1, _p2;
	float _slope;

};

#endif // !SLOPE_H