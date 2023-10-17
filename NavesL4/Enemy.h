#pragma once

#include "Actor.h"
#include "Animation.h" 

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update();
	void impacted(); // Recibe impacto y pone animaci�n de morir
	
	int state;
	float vxIntelligence; //velocidad que de verdad tiene que no toca el motor de fuerzas f�sicas

	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animaci�n mostrada
};
