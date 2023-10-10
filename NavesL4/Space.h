#pragma once

#include "Actor.h"
#include <list>

class Space
{
public:
	Space(float gravity);

	//Coge la lista de elementos dinamico sy los va actualizando
	//Aumenta la gravedad del juego y llama a los moveRight y moveLeft
	void update();

	//Mira el primer elemento din�mico (Jugador) con velocidad vx=40
	//con cada elemento dinamico un for con los estementos estaticos, si la parte izq es mas peque�a q el posible punto q puedas llegar
	//si la izq es mas pequ��a se reduce tu posible movimieinto
	void updateMoveRight(Actor* dynamicAct);
	void updateMoveLeft(Actor* dynamicAct);
	void updateMoveTop(Actor* dynamicAct);
	void updateMoveDown(Actor* dynamicAct);

	void addDynamicActor(Actor* actor); //din�mico
	void addStaticActor(Actor* actor); //est�tico
	void removeDynamicActor(Actor* actor);
	void removeStaticActor(Actor* actor);

	float gravity;
	list<Actor*> dynamicActors;
	list<Actor*> staticActors;
};
