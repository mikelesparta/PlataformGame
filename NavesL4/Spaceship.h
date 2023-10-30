#pragma once

#include "Enemy.h"
#include "Animation.h" 

class Spaceship : public Enemy
{
public:
	Spaceship(float x, float y, int widthIcon, int heightIcon, Game* game);
};

