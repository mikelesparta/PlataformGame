#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map> 

// Valores generales
#define WIDTH 480
#define HEIGHT 320

#include "Layer.h"
class Layer;

class Game
{
public:
	Game();

	void scale();
	void loop();

	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - cache
	TTF_Font* font;
	
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador

	bool scaledToMax = false;
	float scaleLower = 1;

	bool loopActive; // Juego activo

	Layer* layer;
	Layer* gameLayer;
	Layer* menuLayer;

	//Estados y orientaciones
	int const stateMoving = 1;
	int const stateJumping = 2;
	int const stateDying = 3;
	int const stateDead = 4;
	int const stateShooting = 5;

	int const orientationRight = 1;
	int const orientationLeft = 2;

	int currentLevel = 0;
	int finalLevel = 3;

	int input;
	int const inputKeyboard = 1;
	int const inputMouse = 2;
	int const inputGamePad = 3;

};
