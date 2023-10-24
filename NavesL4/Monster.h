#pragma once

#include "Enemy.h"
#include "Animation.h" 

class Monster : public Enemy
{
public:
	Monster(string filename, float x, float y, int widthIcon, int heightIcon, Game* game);
};
