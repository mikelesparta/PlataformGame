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

	// Acabo la animación, no sabemos cual
	//if (endAnimation) {
	//	// Estaba saltando
	//	if (state == game->stateMoving) {
	//		cout << "SALTA" << endl;
	//		state = game->stateDying;
	//	}
	//}

	if (state == game->stateMoving) {
		animation = aMoving;
		cout << "MOVIMIENTO" << endl;
	}
	
	if (state == game->stateDying) {
		animation = aDying;
	}

	state = game->stateDying;
}


void Trampoline::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}

void Trampoline::impacted() {
	if (state != game->stateMoving) {
		state = game->stateMoving;
	}
}