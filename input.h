#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>

using namespace std;


class Input {
public:
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& event);
	void keyDownEvent(const SDL_Event& event);
	
	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

private:
	map<SDL_Scancode, bool> _heldKeys;
	map<SDL_Scancode, bool> _pressedKeys;
	map<SDL_Scancode, bool> _releasedKeys;
};


#endif