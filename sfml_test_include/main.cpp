
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

void playerCustomInput(GameClass* Game, sf::Keyboard::Key key, bool isPressed)
{
	Game::GameObject* player = nullptr;
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
	plita_pressed = 1
};

void plita_pressed_instruction(GameClass* Game, Game::GameObject* sender)
{
	if (sender->getName() == "plita")
	{
		Game::GameObject* OBJ = nullptr;
		if (Game->getObject("test", OBJ))
		{
			std::cout << "object is already exists";
			return;
		}
		else
		{
			OBJ = new Game::GameObject();
			OBJ->setName("test");
			OBJ->setPosition(sf::Vector2f(200, 0));
			OBJ->updateTexture("resources/GameAssets/animations/scene/fire.png");
			OBJ->setFramePerSeconds(48);
			OBJ->setScale(sf::Vector2f(2, 2));
			OBJ->getSFMlobj().setTextureRect(sf::IntRect(0, 0, 64, 64));
			OBJ->enableAnimation();
			OBJ->setVisible(true);
			Game->addObjectonScene(*OBJ, Game::objectType::dynamicType, 0);
			delete OBJ;
		}
		OBJ = nullptr;
	}
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

void plitaScript(GameClass* Game, Game::GameObject* plita)
{
	Game::GameObject* player = nullptr;
	if (Game->getObject("player", player))
	{
		Game::cords playerCordsCheck;
		Game::cords plitaCordsCheck;

		plitaCordsCheck.x = plita->getPosition().x + (plita->getSize().width / 2);
		plitaCordsCheck.y = plita->getPosition().y + (plita->getSize().height - 20);
		
		playerCordsCheck.x = player->getPosition().x + (player->getSize().width / 2);
		playerCordsCheck.y = player->getPosition().y + (player->getSize().height);

		if (playerCordsCheck.x >= plitaCordsCheck.x - 20 and playerCordsCheck.x <= plitaCordsCheck.x + 20 and \
			playerCordsCheck.y >= plitaCordsCheck.y - 10 and playerCordsCheck.y <= plitaCordsCheck.y + 10) 
		{
			Game->emitGameEvent(plita_pressed, plita);
		}
		
		player = nullptr;
	}
	
}

int main()
{

	GameClass game(640, 480, 60);

	game.setPlayerInput(playerCustomInput);

	game.addScript("preview", "player", controlScript); 
	game.addScript("preview", "plita", plitaScript);




	game.addInstruction(plita_pressed, plita_pressed_instruction);

	game.loadScene("resources/scenes/previewScene.txt");
	game.run();
}
