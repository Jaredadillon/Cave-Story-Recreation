#pragma once
#ifndef TILE_H
#define TILE_H

#include "graphics.h"
#include <SDL.h>

class Tile {
public:
	//default constructor
	Tile();
	
	//custom constructor
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);

	//update function for the tile class
	void update(int elapsedTime);
	
	//draw function for the tile class
	void draw(Graphics& graphics);


protected:

	SDL_Texture* _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;

};


#endif // !TILE_H