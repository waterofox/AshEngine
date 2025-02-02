
#include "AshEngine/AshEngine.h"
#include <iostream>
#include <vector>

#define playerSpeed 150

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
using namespace ash;

void playerCustomInput(GameEngine* Game, sf::Keyboard::Key key, bool isPressed)
{
	ash::GameObject* player = nullptr;
	if (!Game->getObject("player", player)) { return; };
	switch (key)
	{
	case sf::Keyboard::D: {(*player)["moveRight"] = std::to_string(int(isPressed)); }break;
	case sf::Keyboard::S: {(*player)["moveDown"] = std::to_string(int(isPressed)); }break;
	case sf::Keyboard::A: {(*player)["moveLeft"] = std::to_string(int(isPressed)); }break;
	case sf::Keyboard::W: {(*player)["moveUp"] = std::to_string(int(isPressed)); }break;
	default:
		break;
	}
}

enum CustomeEvents
{
	plita_pressed = 1
};

void plita_pressed_instruction(GameEngine* Game, ash::GameObject* sender)
{
	if (sender->getName() == "plita")
	{
		ash::GameObject* OBJ = nullptr;
		if (Game->getObject("test", OBJ))
		{
			std::cout << "object is already exists";
			return;
		}
		else
		{
			OBJ = new ash::GameObject();
			OBJ->setName("test");
			OBJ->setPosition(sf::Vector2f(200, 0));
			OBJ->updateTexture("resources/GameAssets/animations/scene/fire.png");
			OBJ->setFramePerSeconds(48);
			OBJ->setScale(sf::Vector2f(2, 2));
			OBJ->getSFMlobj().setTextureRect(sf::IntRect(0, 0, 64, 64));
			OBJ->enableAnimation();
			OBJ->setVisible(true);
			Game->addObjectonScene(*OBJ, ash::objectType::dynamicType, 0);
			delete OBJ;
		}
		OBJ = nullptr;
	}
}

void controlScript(GameEngine* Game,ash::GameObject* player)
{
	player->disableAnimation();
	player->setCurrentFrame(1);

	if (bool(std::stoi((*player)["moveUp"])))
	{
		player->moveY(-playerSpeed*DELTA_TIME.asSeconds());
	}
	if (bool(std::stoi((*player)["moveDown"])))
	{
		player->moveY(playerSpeed * DELTA_TIME.asSeconds());
	}
	if (bool(std::stoi((*player)["moveRight"])))
	{
		player->moveX(playerSpeed * DELTA_TIME.asSeconds());
	}
	if (bool(std::stoi((*player)["moveLeft"])))
	{
		player->moveX(-playerSpeed * DELTA_TIME.asSeconds());
	}

}

void plitaScript(GameEngine* Game, ash::GameObject* plita)
{
	ash::GameObject* player = nullptr;
	if (Game->getObject("player", player))
	{
		ash::cords playerCordsCheck;
		ash::cords plitaCordsCheck;

		plitaCordsCheck.x = plita->getPosition().x + (plita->getSize().width / 2);
		plitaCordsCheck.y = plita->getPosition().y + (plita->getSize().height - 20);
		
		playerCordsCheck.x = player->getPosition().x + (player->getSize().width / 2);
		playerCordsCheck.y = player->getPosition().y + (player->getSize().height);

		if (playerCordsCheck.x >= plitaCordsCheck.x - 20 and playerCordsCheck.x <= plitaCordsCheck.x + 20 and \
			playerCordsCheck.y >= plitaCordsCheck.y - 10 and playerCordsCheck.y <= plitaCordsCheck.y + 10) 
		{
			if ((*plita)["is_active"] == "false")
			{
				(*plita)["is_active"] = "true";
				Game->emitGameEvent(plita_pressed, plita);
			}
		}
		
		player = nullptr;
	}
	
}

int main()
{

	GameEngine game(640, 480, 60);

	game.setPlayerInput(playerCustomInput);

	game.addScript("preview", "player", controlScript); 
	game.addScript("preview", "plita", plitaScript);

	game.addInstruction(plita_pressed, plita_pressed_instruction);

	game.addPropertiesSetsConfig("resources/properties.txt");

	game.loadScene("resources/scenes/previewScene.txt");
	game.run();
}
