#include "sprite.h"

//default constructor
Sprite::Sprite() {}

//custom constructor. create some local variable instances.
Sprite::Sprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	_x(posX),
	_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	//if an image cannot be found, give an error message. otherwise create the texture.
	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) {
		printf("\nError: Unable to load image\n");
	}

	this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {}

//update function
void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y, _sourceRect.w * globals::SPRITE_SCALE, _sourceRect.h * globals::SPRITE_SCALE);
}

//draw the image to the screen
void Sprite::draw(Graphics& graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

//get the bounding box using getter
const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

//determines which side the collision happens on
const sides::Side Sprite::getCollisionSide(Rectangle& other) const {
	int amtRight;
	int amtLeft;
	int amtTop;
	int amtBottom;

	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}
	return
		lowest == abs(amtRight) ? sides::Side::RIGHT :
		lowest == abs(amtLeft) ? sides::Side::LEFT :
		lowest == abs(amtTop) ? sides::Side::TOP :
		lowest == abs(amtBottom) ? sides::Side::BOTTOM :
		sides::Side::NONE;
}

void Sprite::setSourceRectX(int value) {
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value) {
	this->_sourceRect.y = value;
}

void Sprite::setSourceRectH(int value) {
	this->_sourceRect.h = value;
}

void Sprite::setSourceRectW(int value) {
	this->_sourceRect.w = value;
}