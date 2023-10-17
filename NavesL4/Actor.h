#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0, float scrollY = 0);
	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0, float scrollY = 0);
	bool containsPoint(int pointX, int pointY); // contiene punto

	int x;
	int y;
	float vx;
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	bool collisionDown;
	bool outRight;
	bool outLeft;
	bool clicked; // Está pulsado

	Game* game; // referencia al juego
	SDL_Texture* texture;
};

