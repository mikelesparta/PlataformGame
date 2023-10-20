#include "Trampoline.h"

Trampoline::Trampoline(float x, float y, Game* game)
	: Actor("res/trampolin.png", x, y, 34, 32, game) {

	state = game->stateDying;

	aDying = new Animation("res/trampolin.png", width, height,
		34, 32, 6, 1, true, game);
	aMoving = new Animation("res/trampolin_movimiento.png", width, height,
		111, 32, 0, 3, true, game);

	animation = aDying;

	vx = 0;
}

void Trampoline::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();

	if (state == game->stateMoving) {
		animation = aMoving;
	}
	
	if (state == game->stateDying) {
		animation = aDying;
	}

	state = game->stateDying;
}


void Trampoline::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Trampoline::impacted() {
	if (state != game->stateMoving) {
		state = game->stateMoving;
	}
}