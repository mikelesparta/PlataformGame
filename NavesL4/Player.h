#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void jump();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0) override; // Va a sobrescribir

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

	Animation* animation; // Referencia a la animaci�n mostrada

	Audio* audioShoot;
};
