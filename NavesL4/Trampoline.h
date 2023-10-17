#pragma once

#include "Actor.h"
#include "Animation.h" 

class Trampoline : public Actor
{
public:
	Trampoline(float x, float y, Game* game);
	void draw(float scrollX = 0) override;
	void update();
	void impacted();

	int state;

	Animation* aDying;
	Animation* aMoving;
	Animation* animation;
};
