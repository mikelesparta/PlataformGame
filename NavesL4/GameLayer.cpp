#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);

	init();
}

void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);

	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);

	space = new Space(1);
	scrollX = 0;
	scrollY = 0;

	tiles.clear(); // Vaciar por si reiniciamos el juego

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("points", WIDTH * 0.92, HEIGHT * 0.05, game);
	textPoints->content = to_string(points);

	pointsCollectables = 0;
	textCollectables = new Text("collectables", WIDTH * 0.8, HEIGHT * 0.05, game);
	textCollectables->content = to_string(pointsCollectables);

	background = new Background("res/fondo_2.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	backgroundCollectables = new Actor("res/icono_recolectable_mini.png",
		WIDTH * 0.75, HEIGHT * 0.05, 24, 24, game);
	backgroungHearts.push_back(new Actor(HEART_ICON, WIDTH * 0.53, HEIGHT * 0.06, 24, 20, game));
	backgroungHearts.push_back(new Actor(HEART_ICON, WIDTH * 0.6, HEIGHT * 0.06, 24, 20, game));
	backgroungHearts.push_back(new Actor(HEART_ICON, WIDTH * 0.67, HEIGHT * 0.06, 24, 20, game));

	enemies.clear(); 
	projectiles.clear(); 
	collectables.clear();
	trampolines.clear();
	doors.clear();

	loadMap("res/" + to_string(game->currentLevel) + ".txt"); //cargamos el mapa
}

void GameLayer::processControls() {
	//Obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Cambio autom�tico de input
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}

		// Procesar teclas
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}

	// Procesar controles
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

	// Disparar, generar disparo
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
			controlShoot = false;
		}
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		//player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->jump();
	}
	else {
		//player->moveY(0);
	}
}

void GameLayer::update() {
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<Collectable*> deleteCollectables;

	if (pause) {
		return;
	}

	// Nivel superado
	if (cup->isOverlap(player)) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
			cout << "NIVEL SUPERADO" << endl;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;

		init();
	}

	// Jugador se cae
	if (player->y > HEIGHT + 80) {
		cout << "CAISTE" << endl;
		init();
	}

	space->update();
	background->update();
	player->update();

	for (auto const& enemy : enemies) {
		enemy->update();
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	for (auto const& collectable : collectables) {
		collectable->update();
	}

	for (auto const& trampoline : trampolines) {
		trampoline->update();
	}

	for (auto const& door : doors) {
		door->update();
	}

	// Colisones: PLAYER - COLLECTABLE
	for (auto const& collectable : collectables) {
		if (player->isOverlap(collectable) && collectable->received == false) { 
			cout << "Obtenido un recolectable" << endl;

			bool pInList = std::find(deleteCollectables.begin(),
				deleteCollectables.end(),
				collectable) != deleteCollectables.end();

			if (!pInList) {
				deleteCollectables.push_back(collectable);
			}
				
			collectable->state = game->stateDead;
			collectable->received = true;
			pointsCollectables++; //Actualizamos el contador de monedas
			textCollectables->content = to_string(pointsCollectables);
		}
	}

	// Colisiones: PLAYER - TRAMPOLINE
	for (auto const& trampoline : trampolines) {
		if (player->isOverlap(trampoline)) {
			int bajoPlayer = player->y + player->height / 2;
			int trampolineAlto = trampoline->y - trampoline->height / 2;
		
			if (bajoPlayer >= trampoline->y) {
				player->jumpTrampoline();
				trampoline->impacted();
			}
		}
	}

	// Colisiones: PLAYER - DOOR
	for (auto const& door : doors) {
		if (player->isOverlap(door) && controlOpen) {		
			if (!door->opened) {
				door->open();
				Door* destination = findDoorPair(door);
				travelDoor(destination);
			}
			else {
				cout << "Door is LOCKED" << endl;
			}
		}
	}

	// Colisiones: PLAYER - ENEMY
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
						
			//Distinto de muriendo y muerto
			if (enemy->state != game->stateDying && enemy->state != game->stateDead) {
				if (enemy->jumpDamage) { // MONSTRUOS
					int bajoPlayer = player->y + player->height / 2;
					int enemyAlto = enemy->y - enemy->height / 2;

					if (bajoPlayer >= enemy->y) {
						player->loseLife(backgroungHearts);
					}
					else { // Saltar encima de los monstruos
						enemy->impacted();
						player->vy = -5; // Jugador bota
					}
				}
				else { // ALIENS
					player->loseLife(backgroungHearts);					
				}
			} 			

			if (player->lifes <= 0) {
				init();
				return;
			}
		}		
	}

	// Colisiones: ENEMY - PROJECTILE
	for (auto const& projectile : projectiles) {
		//Si la velocidad es 0 lo quitamos, ya que ha chocado con un objeto
		//Cutre lo m�s optimo es ver si el proyectil choca por la izquierda o la derecha
		if (projectile->isInRender(scrollX, scrollY) == false || projectile->vx == 0) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}

				//para que se ponga el estado y empiece a morirse
				enemy->impacted();

				points++;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
		cout << "ELIMINADO" << endl;
	}

	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}

	deleteProjectiles.clear();

	for (auto const& collectable : deleteCollectables) {
		collectables.remove(collectable);
	}

	deleteCollectables.clear();
}

void GameLayer::calculateScroll() {
	//Le damos un margen para que se actualice el mapa
	
	if (doorOpened) {
		if (player->x < mapWidth) {
			scrollX = player->x - WIDTH * 0.7;
		}
		if (player->x > mapWidth){
			scrollX = player->x - WIDTH * 0.3;
		}

		doorOpened = false;
	} 
	else {

		// L�mite izquierda
		if (player->x > WIDTH * 0.3) {
			if (player->x - scrollX < WIDTH * 0.3) {
				scrollX = player->x - WIDTH * 0.3;
			}
		}
		// L�mite derecha
		if (player->x < mapWidth - WIDTH * 0.3) {
			if (player->x - scrollX > WIDTH * 0.7) {
				scrollX = player->x - WIDTH * 0.7;
			}
		}
		// L�mite arriba
		if (player->y > HEIGHT * 0.1) {
			if (player->y - scrollY < HEIGHT * 0.1) {
				scrollY = player->y - HEIGHT * 0.1;
			}
		}
		// L�mite abajo
		if (player->y < mapWidth - HEIGHT * 0.3) {
			if (player->y - scrollY > HEIGHT * 0.7) {
				scrollY = player->y - HEIGHT * 0.7;
			}
		}
	}
}

void GameLayer::draw() {
	calculateScroll();

	background->draw();

	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	for (auto const& door : doors) {
		door->draw(scrollX, scrollY);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}

	cup->draw(scrollX, scrollY);
	player->draw(scrollX, scrollY);

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	for (auto const& collectable : collectables) {
		collectable->draw(scrollX, scrollY);
	}

	for (auto const& trampoline : trampolines) {
		trampoline->draw(scrollX, scrollY);
	}

	for (auto const& heart : backgroungHearts) {
		heart->draw();
	}

	//Set-up displays no se mueven con el scroll
	backgroundPoints->draw();
	backgroundCollectables->draw();
	textPoints->draw();
	textCollectables->draw();

	// HUD - NO tienen scroll, posici�n fija
	if (game->input == game->inputMouse) {
		buttonJump->draw();
		buttonShoot->draw();
		pad->draw();
	}

	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	// Pulsada
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		case SDLK_RETURN: // abre puertas
			controlOpen = true;
			break;
		}
	}

	// Levantada
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		case SDLK_RETURN: // abrir puerta
			controlOpen = false;
			break;
		}
	}
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str()); //abrir fichero

	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Dividir fichero por l�nea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line); //cada l�nea se divide en car�cter
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por car�cter (en cada l�nea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central, tile width = 40
				float y = 32 + i * 32; // y suelo, tile height = 32
				loadMapObject(character, x, y); //creamos el objeto
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {	
	case 'C': {
		cup = new Tile("res/copa.png", x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		cup->y = cup->y - cup->height / 2;

		//No hace falta en el espacio din�mico
		space->addDynamicActor(cup); // Realmente no hace falta
		break;
	}
	case 'E': {
		// Primero a�adir Tile - Fondo en la misma posici�n
		loadMapObject('.', x, y);

		Enemy* enemy = new Spaceship(x, y, 36, 40, game);
		// modificaci�n para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'e': {
		loadMapObject('.', x, y);

		Enemy* enemy = new Monster(x, y, 36, 29, game);
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	//El player es una referencia si no tiene el 1 en el mapa no funcionar�
	case '1': { //Creamos player
		// Primero a�adir Tile - Fondo en la misma posici�n
		loadMapObject('.', x, y);

		player = new Player(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case '#': { //Creamos tile
		Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
		
		// Modificaci�n para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'R': {
		loadMapObject('.', x, y);

		Collectable* collectable = new Collectable(x, y, game);
		collectable->y = collectable->y - collectable->height / 2;
		collectables.push_back(collectable);
		space->addDynamicActor(collectable);
		break;
	}
	case 'Y': {
		//Primero a�adir tile
		loadMapObject('.', x, y);

		Trampoline* trampoline = new Trampoline(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		trampoline->y = trampoline->y - trampoline->height / 2;
		trampolines.push_back(trampoline);
		space->addDynamicActor(trampoline);
		break;
	}
	case '9': {
		//Primero a�adir tile
		loadMapObject('.', x, y);

		Door* door = new Door(x, y, game, 9);
		addDoor(door);
		break;
	}
	case '8': {
		//Primero a�adir tile
		loadMapObject('.', x, y);

		Door* door = new Door(x, y, game, 8);
		addDoor(door);
		break;
	}
	case '7': {
		//Primero a�adir tile
		loadMapObject('.', x, y);

		Door* door = new Door(x, y, game, 7);
		addDoor(door);
		break;
	}
	case '5': {
		//Primero a�adir tile
		loadMapObject('.', x, y);

		Door* door = new Door(x, y, game, 5);
		addDoor(door);
		break;
	}
	case '4': {
		//Primero a�adir tile
		loadMapObject('.', x, y);

		Door* door = new Door(x, y, game, 4);
		addDoor(door);
		break;
	}
	}
}

void GameLayer::addDoor(Door* door) {
	// modificaci�n para empezar a contar desde el suelo.
	door->y = door->y - door->height / 2;
	doors.push_back(door);
	space->addDynamicActor(door);
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificaci�n de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que hacen click, nueva pulsaci�n
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;

		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			// CLICK TAMBIEN TE MUEVE
			controlMoveX = pad->getOrientationX(motionX);
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				pad->clicked = false; // han sacado el rat�n del pad
				controlMoveX = 0;
			}
		}
		else {
			controlMoveX = 0;
		}

		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}

void GameLayer::travelDoor(Door* destination) {
	player->x = destination->x;
	player->y = destination->y;
	destination->opened = true; // Lock door
	doorOpened = true;
	
	for (auto const& door : doors) {
		if (door->numDoor == destination->numDoor) {
			door->close();
		}
	}

	draw();	
}

Door* GameLayer::findDoorPair(Door* openedDoor) {
	for (auto const& door : doors) {
		if (openedDoor->numDoor == door->numDoor) {
			if (door != openedDoor) {
				return door;
			}
		}
	}

	return openedDoor;
}