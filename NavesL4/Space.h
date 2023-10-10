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

	//Mira el primer elemento dinámico (Jugador) con velocidad vx=40
	//con cada elemento dinamico un for con los estementos estaticos, si la parte izq es mas pequeña q el posible punto q puedas llegar
	//si la izq es mas pequéña se reduce tu posible movimieinto
	void updateMoveRight(Actor* dynamicAct);
	void updateMoveLeft(Actor* dynamicAct);
	void updateMoveTop(Actor* dynamicAct);
	void updateMoveDown(Actor* dynamicAct);

	void addDynamicActor(Actor* actor); //dinámico
	void addStaticActor(Actor* actor); //estático
	void removeDynamicActor(Actor* actor);
	void removeStaticActor(Actor* actor);

	float gravity;
	list<Actor*> dynamicActors;
	list<Actor*> staticActors;
};
