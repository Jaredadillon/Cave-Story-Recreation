#include "player.h"
#include "bat.h"

//default constructor
Level::Level() {}

//custom constructor. Assign some local variables and pass the mapName and the graphics object to the loadMap method.
Level::Level(string mapName, Graphics& graphics) :
	_mapName(mapName),
	_size(Vector2(0, 0)) {

	this->LoadMap(mapName, graphics);
}

Level::~Level() {}

//load a ton of stuff
void Level::LoadMap(string mapName, Graphics& graphics) {
	//parse the xml or *.tmx file
	XMLDocument doc;
	stringstream ss;
	ss << mapName << ".tmx";
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	//get the entire width and height of the whole map and store it in _size
	int width, height;

	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//get the width and height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;

	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_size = Vector2(tileWidth, tileHeight);

	//loading the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {

			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			char* path;
			stringstream ss;
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			
			//get animations for the tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.duration = pFrame->Int64Attribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfo.push_back(ati);
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//load the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//load the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//build each individual tile. If the gid is 0, no tile should be drawn and the loop should continue
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//get the tileset for the specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1) {
								//no tileset found
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//get the position of the tile in the level
							int xx, yy = 0;

							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							//calculate the position of the tile in the tileset
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//build the actual tile and add it to the level's list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->_animatedTileInfo.size(); i++) {
								if (this->_animatedTileInfo.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfo.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true) {
								vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}
								AnimatedTile tile(tilesetPositions, ati.duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
							}
							else {
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}
					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
	//parse out collisions from the first object group with the attribute collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x;
						float y;
						float width;
						float height;

						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							ceil(x) * globals::SPRITE_SCALE,
							ceil(y) * globals::SPRITE_SCALE,
							ceil(width) * globals::SPRITE_SCALE,
							ceil(height) * globals::SPRITE_SCALE
						));
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			//parse out all of the slopes from the *.tmx file
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(ceil(pObject->FloatAttribute("x")), ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							vector<string> pairs;
							const char* pointString = pPolyline->Attribute("points");
							//split the polyline coordinates into pairs by the empty space between them.
							stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');

							//loop through the list of pairs and split them into Vector2's and store them in the points vector.
							for (int i = 0; i < pairs.size(); i++) {
								vector<string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(stoi(ps.at(0)), stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < points.size(); i += 2) {
							this->_slopes.push_back(Slope(
								Vector2(
									(p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
									(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),
								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
									(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)));
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			//other objectgroups go here (e.g. spawnpoints)
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(ceil(x) * globals::SPRITE_SCALE, ceil(y) * globals::SPRITE_SCALE);
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "doors") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty) {
										const char* name = pProperty->Attribute("name");
										stringstream ss;
										ss << name;
										if (ss.str() == "destination") {
											const char* value = pProperty->Attribute("value");
											stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str());
											this->_doorList.push_back(door);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						stringstream ss;
						ss << name;
						if (ss.str() == "bat") {
							this->_enemies.push_back(new Bat(graphics, Vector2(floor(x) * globals::SPRITE_SCALE, floor(y) * globals::SPRITE_SCALE)));
						}
					
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = (gid - 1) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	int tsyy = 0;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

	return finalTilesetPosition;
}

void Level::update(int elapsedTime, Player& player) {
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->update(elapsedTime, player);
	}
}

void Level::draw(Graphics& graphics) {
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}
}

//check for tile collisions. Get a list of collision rectangles and compare it with sprite objects to see if they are 'colliding' send it back.
vector<Rectangle> Level::checkTileCollisions(const Rectangle& other) {
	vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

//check for slope collisions. Get a list of rectangles and compare it with sprite objects to see if they are 'colliding' send it back.
vector<Slope> Level::checkSlopeCollisions(const Rectangle& other) {
	vector<Slope> others;
	for (int i = 0; i < this->_slopes.size(); i++) {
		if (this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

//check for door collisions. Get a list of rectangles and compare it with the sprite objects to see if they are 'colliding' send it back.
vector<Door> Level::checkDoorCollisions(const Rectangle& other) {
	vector<Door> others;
	for (int i = 0; i < this->_doorList.size(); i++) {
		if (this->_doorList.at(i).collidesWith(other)) {
			others.push_back(this->_doorList.at(i));
		}
	}
	return others;
}

//check for enemy collisions. Get a list of rectangles and compare it with the sprite objects to see if they are 'colliding' send it back.
vector<Enemy*> Level::checkEnemyCollisions(const Rectangle& other) {
	vector<Enemy*> others;
	for (int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}

//grab the spawnpoint and send it back to the private variable _spawnPoint
const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}