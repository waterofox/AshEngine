
#include "ShLlib/ShLlib.h"
#include <iostream>
#include <vector>

enum textures
{
	PLAYER_RIGHT_WALK = 0,
	PLAYER_LEFT_WALK = 1,
	PLAYER_UP_WALK = 2,
	PLAYER_DOWN_WALK = 3,
	PLITA_UNPRESSED = 4,
	PLITA_PRESSED = 5

};

std::vector<std::string> texture{ "resources/GameAssets/animations/player/playerSideRightWalk.png",
									"resources/GameAssets/animations/player/playerSideLeftWalk.png",
									"resources/GameAssets/animations/player/playerBackWalk.png",
									"resources/GameAssets/animations/player/playerFrontWalk.png",
									"resources/GameAssets/statickAssets/buildings/disPlita.png",
									"resources/GameAssets/statickAssets/buildings/enaPlita.png" };

enum CustomeEvents
{
	plita_pressed = 1
};
void plita_pressed_instruction(GameClass* Game, Game::GameObject* sender)
{
	Game::GameObject* OBJECT = nullptr;
	//todo ДОБАВЬ УЖЕ visible для ОБЪЕКТОВ!
	if (sender->getName() == "plita_1")
	{
		if (!Game->getObject("desc", OBJECT)) { return; }
		OBJECT->setX(sender->getPosition().x + 100);
		
		Game->getObject("plita_1", OBJECT);
		OBJECT->updateTexture(texture[PLITA_PRESSED]);
	}
	if (sender->getName() == "plita_2")
	{
		Game->getObject("plita_2", OBJECT);
		OBJECT->updateTexture(texture[PLITA_PRESSED]);
	}
	OBJECT = nullptr;
}

void controlScript(GameClass* Game,Game::GameObject* OBJECT)
{
	if (!(OBJECT->moveDown or OBJECT->moveUp or OBJECT->moveRight or OBJECT->moveLeft))
	{
		if (OBJECT->isAnimated())
		{
			OBJECT->disableAnimation();
			OBJECT->setCurrentFrame(1);
		}
	}
	else
	{
		if (!OBJECT->isAnimated()) { OBJECT->enableAnimation(); }
	}

	//movement
	if (OBJECT->moveRight) {

		if (OBJECT->getTexture() != texture[PLAYER_RIGHT_WALK])
		{
			OBJECT->updateTexture(texture[PLAYER_RIGHT_WALK]);
		}
		OBJECT->moveX(150 * DELTA_TIME.asSeconds());
	}
	if (OBJECT->moveLeft) {
		if (OBJECT->getTexture() != texture[PLAYER_LEFT_WALK])
		{
			OBJECT->updateTexture(texture[PLAYER_LEFT_WALK]);
		}
		OBJECT->moveX(-1 * 150 * DELTA_TIME.asSeconds());
	}
	if (OBJECT->moveUp) {
		if (OBJECT->getTexture() != texture[PLAYER_UP_WALK])
		{
			OBJECT->updateTexture(texture[PLAYER_UP_WALK]);
		}
		OBJECT->moveY(-1 * 150 * DELTA_TIME.asSeconds());
	}
	if (OBJECT->moveDown) {
		if (OBJECT->getTexture() != texture[PLAYER_DOWN_WALK])
		{
			OBJECT->updateTexture(texture[PLAYER_DOWN_WALK]);
		}
		OBJECT->moveY(150 * DELTA_TIME.asSeconds());
	}
}
/*
void plitaScript(GameClass* Game)
{
	int countOfPLits = 2;
	std::string objName = "plita_";
	Game::cords plitaCenter;
	Game::cords playerCheck;

	for (int i = 0; i < 2; ++i)
	{
		if (!Game->getObject(objName + std::to_string(i + 1), OBJECT)) { continue; }
		Game::GameObject* player = nullptr;
		if (!Game->getObject("player", player)) { continue; }
		// центр плиты
		plitaCenter.x = OBJECT->getPosition().x + (OBJECT->getSize().width) / 2;
		plitaCenter.y = OBJECT->getPosition().y + OBJECT->getSize().height-20;
		//серидина нижней границы игрока
		playerCheck.x = player->getPosition().x + (player->getSize().width) / 2;
		playerCheck.y = player->getPosition().y + player->getSize().height;

		if (playerCheck.x >= plitaCenter.x - 20 and playerCheck.x <= plitaCenter.x + 20 and \
			playerCheck.y <= plitaCenter.y + 10 and playerCheck.y >= plitaCenter.y - 10)
		{
			Game->emitGameEvent(plita_pressed, OBJECT);
		}
	}
}*/
int main() 
{

	GameClass game(640,480,60);
	game.addScript("preview", "player", controlScript); //todo блять. скрипты должны цепляться на объекты, а не на сцену
	//game.addScript("preview", plitaScript);

	game.addInstruction(plita_pressed, plita_pressed_instruction);

	game.loadScene("resources/scenes/previewScene.txt");
	game.run();
}
