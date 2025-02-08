
#include "AshEngine/AshEngine.h"
#include <iostream>
#include <vector>

#define playerSpeed 100

enum textures
{
	PLAYER_RIGHT_WALK = 0,
	PLAYER_LEFT_WALK = 1,
	PLAYER_UP_WALK = 2,
	PLAYER_DOWN_WALK = 3,
	PLITA_UNPRESSED = 4,
	PLITA_PRESSED = 5,
	SC = 6

};

std::vector<std::string> texture{ "resources/GameAssets/animations/player/playerSideRightWalk.png",
									"resources/GameAssets/animations/player/playerSideLeftWalk.png",
									"resources/GameAssets/animations/player/playerBackWalk.png",
									"resources/GameAssets/animations/player/playerFrontWalk.png",
									"resources/GameAssets/statickAssets/buildings/disPlita.png",
									"resources/GameAssets/statickAssets/buildings/enaPlita.png" ,
									"resources/GameAssets/statickAssets/terrain/sq.png"};
using namespace ash;

void playerCustomInput(GameEngine* Game, sf::Keyboard::Key key, bool isPressed)
{
	ash::GameObject* player = nullptr;
	if (!Game->getObject("player", player)) { return; };
	switch (key)
	{
	case sf::Keyboard::D: {player->moveRight = isPressed; }break;
	case sf::Keyboard::S: {player->moveDown = isPressed; }break;
	case sf::Keyboard::A: {player->moveLeft = isPressed; }break;
	case sf::Keyboard::W: {player->moveUp = isPressed; }break;
	default:
		break;
	}
}

enum CustomeEvents
{
	plita_pressed = 1,
	key_pressed = 2,
};

void plita_pressed_instruction(GameEngine* Game, ash::GameObject* sender)
{
	std::string color;
	for (int i = 0; i < sender->getName().length(); ++i)
	{
		if (sender->getName()[i] == '_')
		{
			for (int j = i + 1; j < sender->getName().length(); ++j)
			{
				color += sender->getName()[j];
			}
			break;
		}
	}
	ash::GameObject* light = nullptr;
	if (Game->getObject("light_" + color, light))
	{
		(*light)["timer_sec"] = '0';
		light->setCurrentFrame(1);
	}
}
void key_pressed_instruction(GameEngine* GAme, ash::GameObject* sender)
{
	GameObject* key = nullptr;
	if (sender->moveUp)
	{
		if (GAme->getObject("Wkey", key))
		{
			(*key)["is_active"] = "true";
		}
	}
	else
	{
		if (GAme->getObject("Wkey", key))
		{
			(*key)["is_active"] = "false";
		}
	}
	if (key != nullptr) { key = nullptr; }
	if (sender->moveLeft)
	{
		if (GAme->getObject("Akey", key))
		{
			(*key)["is_active"] = "true";
		}
	}
	else
	{
		if (GAme->getObject("Akey", key))
		{
			(*key)["is_active"] = "false";
		}
	}
	if (key != nullptr) { key = nullptr; }
	if (sender->moveDown)
	{
		if (GAme->getObject("Skey", key))
		{
			(*key)["is_active"] = "true";
		}
	}
	else
	{
		if (GAme->getObject("Skey", key))
		{
			(*key)["is_active"] = "false";
		}
	}
	if (key != nullptr) { key = nullptr; }
	if (sender->moveRight)
	{
		if (GAme->getObject("Dkey", key))
		{
			(*key)["is_active"] = "true";
		}
	}
	else
	{
		if (GAme->getObject("Dkey", key))
		{
			(*key)["is_active"] = "false";
		}
	}
	if (key != nullptr) { key = nullptr; }
}

void colScript(GameEngine* Game, ash::GameObject* col)
{
	GameObject* player = nullptr;
	if (Game->getObject("player", player))
	{
		col->getSFMlobj().setTextureRect(sf::IntRect(0,0, player->getCollisionSize().width, player->getCollisionSize().height));
		col->setPosition(sf::Vector2f(player->getPosition().x + player->getCollisionCenter().x, player->getPosition().y + player->getCollisionCenter().y));
		player = nullptr;
	}
}
void keyScript(GameEngine* Game, ash::GameObject* key)
{
	if ((*key)["is_active"] == "true")
	{
		key->setCurrentFrame(1);
	}
	else
	{
		key->setCurrentFrame(0);
	}
}
void controlScript(GameEngine* Game,ash::GameObject* player)
{
	bool moveFlag = false;
	int frame;
	if (player->moveUp)
	{
		player->move(0,-playerSpeed*DELTA_TIME);
		if (player->getTexturePath() != texture[textures::PLAYER_UP_WALK])
		{
			frame = player->getCurrentFrame();
			player->loadTexture(texture[textures::PLAYER_UP_WALK]);
			player->setCurrentFrame(frame);
		}
		moveFlag = true;
	}
	if (player->moveDown)
	{
		player->move(0,playerSpeed * DELTA_TIME);
		if (player->getTexturePath() != texture[textures::PLAYER_DOWN_WALK])
		{
			frame = player->getCurrentFrame();
			player->loadTexture(texture[textures::PLAYER_DOWN_WALK]);
			player->setCurrentFrame(frame);
		}
		moveFlag = true;
	}
	if (player->moveRight)
	{
		player->move(playerSpeed * DELTA_TIME,0);
		if (player->getTexturePath() != texture[textures::PLAYER_RIGHT_WALK])
		{
			frame = player->getCurrentFrame();
			player->loadTexture(texture[textures::PLAYER_RIGHT_WALK]);
			player->setCurrentFrame(frame);
		}
		moveFlag = true;
	}
	if (player->moveLeft)
	{
		player->move(-playerSpeed * DELTA_TIME,0);
		if (player->getTexturePath() != texture[textures::PLAYER_LEFT_WALK])
		{
			frame = player->getCurrentFrame();
			player->loadTexture(texture[textures::PLAYER_LEFT_WALK]);
			player->setCurrentFrame(frame);
		}
		moveFlag = true;
	}
	if (!moveFlag)
	{
		player->disableAnimation();
		player->setCurrentFrame(1);
	}
	else
	{
		player->enableAnimation();
	}
	Game->emitGameEvent(key_pressed,player);
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
				plita->loadTexture(texture[textures::PLITA_PRESSED]);
			}
		}
		else
		{
			if ((*plita)["is_active"] == "true")
			{
				(*plita)["is_active"] = "false";
				plita->loadTexture(texture[textures::PLITA_UNPRESSED]);
			}
		}
		
		player = nullptr;
	}
	
}
void lightScript(GameEngine* Game, ash::GameObject* light)
{
	if (light->getCurrentFrame() != 0)
	{
		float time = std::stof((*light)["timer_sec"]);
		time += DELTA_TIME;
		if (time >= 2)
		{
			light->setCurrentFrame(0);
		}
		else
		{
			(*light)["timer_sec"] = std::to_string(time);
		}
	}
}

int main()
{

	GameEngine game(640, 480, 120);

	game.setPlayerInput(playerCustomInput);

	game.addScript("lights", "player", controlScript); 

	game.addScript("lights", "plita_green", plitaScript);
	game.addScript("lights", "plita_red", plitaScript);
	game.addScript("lights", "plita_blue", plitaScript);
	game.addScript("lights", "plita_yellow", plitaScript);

	game.addScript("lights", "light_green", lightScript);
	game.addScript("lights", "light_red", lightScript);
	game.addScript("lights", "light_blue", lightScript);
	game.addScript("lights", "light_yellow", lightScript);

	game.addScript("lights", "col2", colScript);

	game.addScript("lights", "Wkey", keyScript);
	game.addScript("lights", "Akey", keyScript);
	game.addScript("lights", "Skey", keyScript);
	game.addScript("lights", "Dkey", keyScript);

	game.addInstruction(plita_pressed, plita_pressed_instruction);
	game.addInstruction(key_pressed, key_pressed_instruction);

	game.addPropertiesSetsConfig("resources/properties.txt");

	game.loadScene("resources/scenes/lightsScene.txt");
	game.startGame();
}
