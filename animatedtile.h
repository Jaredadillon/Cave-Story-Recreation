#pragma once
#ifndef ANIMATED_TILE
#define ANIMATED_TILE

#include "tile.h"
#include <vector>

using namespace std;

class AnimatedTile : public Tile {
public:
	AnimatedTile(vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics& graphics);

protected:
	int _amountOfTime = 0;
	int _notDone = false;

private:
	vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;
};

struct AnimatedTileInfo {
public:
	int TilesetsFirstGid;
	int StartTileId;
	vector<int> TileIds;
	int duration;
};


#endif // !ANIMATED_TILE