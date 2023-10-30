#pragma once

#include "Actor.h"
#include "Animation.h" 

class Door : public Actor
{
public:
	Door(float x, float y, Game* game, int num);
	void draw(float scrollX = 0, float scrollY = 0) override;
	void update();
	void open();
	void close();

	int state;
	int numDoor;
	
	bool opened;

	Animation* aDying;
	Animation* aMoving;
	Animation* animation;
};
