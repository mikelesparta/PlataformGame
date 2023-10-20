#include "Door.h"


Door::Door(float x, float y, Game* game, int num)
	: Actor("res/puerta.png", x, y, 35, 58, game) {

	state = game->stateDying;
	numDoor = num;

	aDying = new Animation("res/puerta.png", width, height,
		35, 58, 6, 1, true, game);
	aMoving = new Animation("res/puerta_abierta.png", width, height,
		261, 58, 6, 5, true, game);

	animation = aDying;

	vx = 0;
}

void Door::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();

	if (endAnimation) {
		// Estaba abriendose
		if (state == game->stateMoving) {
			state = game->stateDying;
			cout << "CIERRATE" << endl;
		}
	}

	if (state == game->stateMoving) {
		animation = aMoving;
	}

	if (state == game->stateDying) {
		animation = aDying;
	}

	
}


void Door::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Door::open() {
	if (state != game->stateMoving) {
		state = game->stateMoving;
		opened = true;
		cout << "ABRIR" << endl;
	}
}

void Door::close() {
	if (state != game->stateDying) {
		state = game->stateDying;
		cout << "CERRAR" << endl;
	}
}