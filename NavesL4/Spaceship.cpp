#pragma once

#include "Spaceship.h"
#include "Enemy.h"
#include "Actor.h"

Spaceship::Spaceship(string filename, float x, float y, int widthIcon, int heightIcon, Game* game)
	: Enemy(ICONO_ENEMIGO, x, y, 36, 40, game) {

	state = game->stateMoving;

	aDying = new Animation(ENEMIGO_MORIR, width, height,
		280, 40, 6, 8, false, game); //animación finita

	aMoving = new Animation(ENEMIGO_MOV, width, height,
		108, 40, 6, 3, true, game);

	animation = aMoving;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;
}
