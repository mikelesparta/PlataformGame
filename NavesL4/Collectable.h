#pragma once

#include "Actor.h"
#include "Animation.h" 

class Collectable : public Actor
{
public:
	Collectable(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update();
	void impacted(); 

	int state;
	bool received;

	Animation* aMoving;
	Animation* animation; 
};