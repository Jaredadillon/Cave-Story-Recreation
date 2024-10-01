#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "globals.h"
#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

//create graphics class
class Graphics {

//public methods are available to all parts of the application
public:
	//graphics object constructor
	Graphics();
	~Graphics();

	//loads image file to eventually store in the _spriteSheets map. Every image is only loaded once. Returns that image from the map rather than the asset directory.
	SDL_Surface* loadImage(const string &filePath);

	//overlaps texture pulled from the sprite sheet onto a particular part of the screen.
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

	//push the render to the screen
	void flip();

	//clears the screen
	void clear();

	SDL_Renderer* getRenderer() const;

//private methods are available only to the class or any subclass derived from that class
private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	map<string, SDL_Surface*> _spriteSheets;
};

#endif