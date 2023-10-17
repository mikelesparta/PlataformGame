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

#include <list>
#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);

	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void loadMap(string name); // Carga el fichero del mapa
	void loadMapObject(char character, float x, float y); //crea el mapa con el fcihero cargado
	void calculateScroll();

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
	int mapWidth;

	list<Tile*> tiles; //donde estarán los suelos y muros
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

	Tile* cup; // Elemento de final de nivel

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


};

