#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

#include <list>

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void jump();
	void jumpTrampoline();
	void teletransport();

	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void loseLife(list<Actor*> &backgroungHearts);

	int lifes = 3;
	int invulnerableTime = 0;

	int shootCadence = 30;
	int shootTime = 0;
	int orientation;
	int state;
	bool onAir;

	Animation* aIdleRight; //cuando estas parado
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;

	Animation* aShootingRight;
	Animation* aShootingLeft;

	Animation* animation; // Referencia a la animación mostrada

	Audio* audioShoot;
};

