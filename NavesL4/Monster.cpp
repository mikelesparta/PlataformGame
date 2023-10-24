#pragma once

#include "Monster.h"
#include "Enemy.h"
#include "Actor.h"

Monster::Monster(string filename, float x, float y, int widthIcon, int heightIcon, Game* game)
	: Enemy(ICONO_ENEMIGO2, x, y, 36, 40, game) {

	state = game->stateMoving;

	aDying = new Animation(ENEMIGO2_MORIR, width, height,
		36, 36, 6, 1, false, game); //animación finita

	aMoving = new Animation(ENEMIGO2_MOV, width, height,
		274, 29, 6, 5, true, game);

	animation = aMoving;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;
}
