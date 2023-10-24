#pragma once

#include "Actor.h"
#include "Animation.h" 

#define ICONO_ENEMIGO "res/enemigo.png"
#define ICONO_ENEMIGO2 "res/enemigo2.png"
#define ENEMIGO_MORIR "res/enemigo_morir.png"
#define ENEMIGO2_MORIR "res/enemigo2_morir.png"

#define ENEMIGO_MOV "res/enemigo_movimiento.png"
#define ENEMIGO2_MOV "res/enemigo2_movimiento.png"
#define ENEMIGO2_IZQ "res/enemigo2_izq.png"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	virtual void update();
	void impacted(); // Recibe impacto y pone animación de morir
	
	int state;
	float vxIntelligence; //velocidad que de verdad tiene que no toca el motor de fuerzas físicas

	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
};
