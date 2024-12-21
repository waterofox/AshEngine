
#include "ShLlib/ShLlib.h"
#include <iostream>

enum CustomeEvents
{
	begin_event = 1,
};
void begin_event_instruction(GameClass* Game, Game::GameObject* sender)
{
	std::cout << "EVENT; object is on x > 0 cord" << '\n';
}

void controlScript(GameClass* Game)
{
	if (!Game->getObject("player", OBJECT)) { return; }
	if (OBJECT->moveRight) { OBJECT->moveX(150 * DELTA_TIME.asSeconds()); }
	if (OBJECT->moveLeft) { OBJECT->moveX(-1 * 150 * DELTA_TIME.asSeconds()); }
	if (OBJECT->moveUp) { OBJECT->moveY(-1 * 150 * DELTA_TIME.asSeconds()); }
	if (OBJECT->moveDown) { OBJECT->moveY(150 * DELTA_TIME.asSeconds()); }

	//std::cout << OBJECT->getPosition().x << std::endl;

	if (int(OBJECT->getPosition().x) <= 0)
	{
		Game->emitGameEvent(begin_event, OBJECT);
	}

	OBJECT = nullptr;
}


int main() 
{
	GameClass game(640,480,60);
	game.addScript("test", controlScript); //Сёме не нравится то, что скрипты нужно добавлять к конктретной сцене
	game.addInstruction(begin_event, begin_event_instruction);


	game.loadScene("resources/scenes/testScene.txt");
	game.run();
}
