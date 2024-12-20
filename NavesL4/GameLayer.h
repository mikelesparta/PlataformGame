#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Tile.h"
#include "Text.h"
#include "Audio.h"
#include "Space.h"
#include "Pad.h"
#include "Collectable.h"
#include "Trampoline.h"
#include "Door.h"
#include "Spaceship.h"
#include "Monster.h"

#include <list>
#include <fstream> // Leer ficheros
#include <sstream> // Leer l�neas / String

#define HEART_ICON "res/corazon.png"

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);

	void init() override;
	void travelDoor(Door* door);
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void loadMap(string name); // Carga el fichero del mapa
	void loadMapObject(char character, float x, float y); //crea el mapa con el fcihero cargado
	void calculateScroll();
	void addDoor(Door* door);
	Door* findDoorPair(Door* door);

	Actor* message;
	bool pause;
	bool controlContinue = false;

	// Elementos de interfaz
	// botones de mentira para controlar el juego
	Pad* pad;

	Actor* buttonJump;
	Actor* buttonShoot;

	Space* space;
	float scrollX;
	float scrollY;
	int mapWidth;

	list<Tile*> tiles; //donde estar�n los suelos y muros
	Audio* audioBackground;
	Text* textPoints;
	Text* textCollectables;
	int points;
	int pointsCollectables;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundCollectables;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<Collectable*> collectables;
	list<Trampoline*> trampolines;
	list<Door*> doors;
	list<Actor*> backgroungHearts;

	Tile* cup; // Elemento de final de nivel

	bool controlShoot = false;
	bool controlOpen = false;
	bool doorOpened = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
};

