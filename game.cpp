#include "game.h"

//create a namespace to hold values for FPS and Max Frame Time
namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

//Game constructor
Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

//Game deconstructor
Game::~Game() {

}

//initialize game loop objects
void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	this->_level = Level("map 1", graphics);
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	this->_hud = HUD(graphics, this->_player);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//start the game loop timer
	while (true) {
		input.beginNewFrame();

		//beginning of the main game loop
		if (SDL_PollEvent(&event)) {
			//check to see if a key has been pressed. If so, create a keyDown event
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			//check to see if a key has been released. If so, create a keyUp event
			if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			if (event.type == SDL_QUIT) {
				return;
			}
		}
		//if the key pressed was ESC, kill the window
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}

		//if the key held down was 'left', move the player left
		if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
			this->_player.moveLeft();
		}
		//if the key held down was 'right', move the player right
		if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			this->_player.moveRight();
		}
		//if either the 'left' or 'right' key is released, kill player movement
		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.stopMoving();
		}

		//check to see if the key held is 'up'
		if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
			this->_player.lookUp();
		}
		//check to see if the key released was 'up'
		if (input.wasKeyReleased(SDL_SCANCODE_UP) == true) {
			this->_player.stopLookingUp();
		}
		//check to see it the key held is 'down'
		if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
			this->_player.lookDown();
		}
		//check to see if the key released was 'down'
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN) == true) { //wasKeyReleased
			this->_player.stopLookingDown();
		}
		//check to see if the key being held is 'space' or 'z'
		if (input.isKeyHeld(SDL_SCANCODE_Z) || input.isKeyHeld(SDL_SCANCODE_SPACE) == true) {
			this->_player.jump();
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->_graphics = graphics;
		this->update(min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

//define the draw method
void Game::draw(Graphics& graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	this->_player.draw(graphics);
	this->_hud.draw(graphics);
	graphics.flip();
}

//define the update method
void Game::update(float elapsedTime) {
	this->_level.update(elapsedTime, this->_player);
	this->_player.update(elapsedTime);
	this->_hud.update(elapsedTime, this->_player);

	//check for collisions
	vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//handles player collisions with tiles
		this->_player.handleTileCollisions(others);
	}

	//check for and handle slope collisions
	vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
		//handles player collisions with slopes
		this->_player.handleSlopeCollisions(otherSlopes);
	}

	//check for and handle door collisions
	vector<Door> otherDoors;
	if ((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleDoorCollisions(otherDoors, this->_level, this->_graphics);
	}

	//check for and handle enemy collisions
	vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleEnemyCollisions(otherEnemies);
	}
}