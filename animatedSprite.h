#pragma once
#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "sprite.h"
#include <vector>
#include <map>


//is the class for spinning off animated sprite objects

class AnimatedSprite : public Sprite {
public:
	//default constructor
	AnimatedSprite();

	//custom constructor
	AnimatedSprite(Graphics& graphics, const string& filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate);

	//play the animation if the animation isn't already playing
	void playAnimation(string animation, bool once = false);

	//update the animated sprite timer
	void update(int elapsedTime);

	//draws the animated sprite to the screen
	void draw(Graphics& graphics, int x, int y);

protected:

	double _timeToUpdate;
	bool _currentAnimationOnce;
	string _currentAnimation;

	//add the animation to the map of animations sprites
	void addAnimation(int frames, int x, int y, string name, int width, int height, Vector2 offset);

	//resets all animations associated with the sprite
	void resetAnimations();

	//stops the current animation
	void stopAnimation();

	//sets a boolean value related to animation visibility
	void setVisible(bool visible);

	//refers to the logic of what happens after an animation is finished
	virtual void animationDone(string currentAnimation) = 0;

	//required function to stage all sprite animations
	virtual void setupAnimations() = 0;

private:
	//map to hold the list of frames for animation
	map<string, vector<SDL_Rect>> _animations;
	//map to hold the offsets
	map<string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed = 0;
	bool _visible;
};


#endif // ! ANIMATED_SPRITE_H
