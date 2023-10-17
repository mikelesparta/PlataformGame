#include "Collectable.h"

Collectable::Collectable(float x, float y, Game* game)
	: Actor("res/recolectable.png", x, y, 36, 40, game) {

	state = game->stateMoving;

	aMoving = new Animation("res/recolectable.png", width, height,
		256, 32, 6, 8, true, game);

	animation = aMoving;

	vx = 0;
	vy = -1; // contrarestar la gravedad
}

void Collectable::update() {
	// Actualizar la animaci�n
	bool endAnimation = animation->update();

	if (state == game->stateMoving) {
		animation = aMoving;
	}

	// Establecer velocidad
	vx = 0;
	vy = -1; // contrarestar la gravedad
}


void Collectable::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}

void Collectable::impacted() {
	if (state != game->stateDying) {
		state = game->stateDead;
	}
}