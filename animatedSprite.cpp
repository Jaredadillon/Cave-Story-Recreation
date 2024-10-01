#include "animatedSprite.h"

//Animation sprite implementation

//default constructor
AnimatedSprite::AnimatedSprite() {}

//custom constructor and set some local variable instances
AnimatedSprite::AnimatedSprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate) :
	//create a sprite object
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	//no animation
	_frameIndex(0),
	//the time to move from one frame/texture to another
	_timeToUpdate(timeToUpdate),
	//make it visible
	_visible(true),
	//no animation has started / reset
	_currentAnimationOnce(false),
	//empty placeholder for the name of the animation ex. runLeft, runRight, etc.
	_currentAnimation("")
{}

//plays the animation
void AnimatedSprite::playAnimation(string animation, bool once) {
	this->_currentAnimationOnce = once;
	//if animation is not currently running, then run it and set frameIndex to 0 so that it starts at the beginning
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

//informs the application of the time between frames of the animation / timeToUpdate
void AnimatedSprite::update(int elapsedTime) {
	//run the sprite update function
	Sprite::update();

	//set some local variable instances
	this->_timeElapsed += elapsedTime;
	//if the time elapsed is > than the time to update take the time elapsed from the time to update
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		//as long as we're moving correctly through the animation, everything is good. Once we reach the end of the animation we need to gracefully stop and that it has the capability to run indefinitely. -1 is to make sure we're just using the contents of the vector and not an empty value.
		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
			this->_frameIndex++;
		}
		//turn off the current animation, make it disappear and stop the animation.
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

//sets the coordinates and size of the rectangle to draw on the screen
void AnimatedSprite::draw(Graphics& graphics, int x, int y) {
	if (this->_visible) {
		SDL_Rect destinationRectangle;
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;

		//grabs the source rectangle information and passes it to the blitSurface method of the graphics class
		SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}

//grabs the frame and stores it in a vector of rectangles.
void AnimatedSprite::addAnimation(int frames, int x, int y, string name, int width, int height, Vector2 offset) {
	//creates an empty vector of rectangles
	vector<SDL_Rect> rectangles;
	//for each frame of the animation grab a rectangle of a specified height and store it in the vector declared above. Each subsequent rectangle will start at the ending 'x' or 'y' location of the previous rectangle.
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}
	//take rectangles and store it in the animations map. Store it by name and by rectangles vector. Use 'pair' to make sure they stay together.
	this->_animations.insert(pair <string, vector<SDL_Rect>>(name, rectangles));
	//if I have offsets, then store them as well.
	this->_offsets.insert(pair<string, Vector2>(name, offset));
}

//resets all animations for the sprite. Clear the animations and offsets map
void AnimatedSprite::resetAnimations() {
	this->_animations.clear();
	this->_offsets.clear();
}

//stop the current animation, reset the frameIndex
void AnimatedSprite::stopAnimation() {
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}

//set visibility of the animation to visible
void AnimatedSprite::setVisible(bool visible) {
	this->_visible = visible;
}

/*
void AnimatedSprite::setupAnimations() {
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
}

void AnimatedSprite::animationDone(string currentAnimation) {

}
*/