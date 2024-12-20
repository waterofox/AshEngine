
#include "GameClass.h"
#include <iostream>

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
	game.addScript("test", controlScript);
	game.loadScene("resources/scenes/testScene.txt");
	game.run();
}
