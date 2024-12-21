
#include "ShLlib/ShLlib.h"
#include <iostream>

enum CustomeEvents
{
	begin_event = 1,
};
void begin_event_instruction(GameClass* Game, Game::GameObject* sender)
{
	if (sender->getPosition().x == 0) 
	{
		std::cout << "EVENT; object is on zero x cord" << '\n';
	}
}

void controlScript(GameClass* Game)
{
	if (!Game->getObject("player", OBJECT)) { return; }
	if (OBJECT->moveRight) { OBJECT->moveX(150 * DELTA_TIME.asSeconds()); }
	if (OBJECT->moveLeft) { OBJECT->moveX(-1 * 150 * DELTA_TIME.asSeconds()); }
	if (OBJECT->moveUp) { OBJECT->moveY(-1 * 150 * DELTA_TIME.asSeconds()); }
	if (OBJECT->moveDown) { OBJECT->moveY(150 * DELTA_TIME.asSeconds()); }
}


int main() 
{
	GameClass game(640,480,60);
	game.addScript("test", controlScript); //Сёме не нравится то, что скрипты нужно добавлять к конктретной сцене


	game.loadScene("resources/scenes/testScene.txt");
	game.run();
}
