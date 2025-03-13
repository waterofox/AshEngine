#include "AshEngine/AshCore.h"
using namespace ash;

#define playerSpeed 100

void catchKeyBoardEvent(AshCore& core,sf::Keyboard::Key key, bool arg)
{
	AshEntity& player = core.getEntity("player");
	switch (key)
	{
	case sf::Keyboard::W: {player.moveUp = arg; }break;
	case sf::Keyboard::A: {player.moveLeft = arg; }break;
	case sf::Keyboard::S: {player.moveDown = arg; }break;
	case sf::Keyboard::D:{player.moveRight = arg;}break;
	default:
		break;
	}
}
void eventHandlingFunction(AshCore& theCore)
{
	sf::RenderWindow& window = theCore.getWindow();
	sf::Event& actualEvent = theCore.getActualEvent();
	if(window.pollEvent(actualEvent))
	{
		AshEntity& player = theCore.getEntity("player");
		switch (actualEvent.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed: {catchKeyBoardEvent(theCore, actualEvent.key.code, true); } break;
		case sf::Event::KeyReleased: {catchKeyBoardEvent(theCore, actualEvent.key.code, false); } break;
		default:
			break;
		}
	}
}

void testScript(AshCore* core, AshEntity& entity)
{
	AshResourceManager& manager = core->getResourceManager();
	sf::Texture& playerTexture = manager.getTexture(entity.getTexturePath());
	bool moveFlag = false;
	playerTexture.setSmooth(!moveFlag);
	if (entity.moveUp)
	{
		entity.move(sf::Vector2f(0, -playerSpeed) * core->getDeltaTime().asSeconds());
		moveFlag = true;
	}
	if (entity.moveRight)
	{
		entity.move(sf::Vector2f(playerSpeed, 0) * core->getDeltaTime().asSeconds());
		moveFlag = true;
	}
	if (entity.moveDown)
	{
		entity.move(sf::Vector2f(0, playerSpeed) * core->getDeltaTime().asSeconds());
		moveFlag = true;
	}
	if (entity.moveLeft)
	{
		entity.move(sf::Vector2f(-playerSpeed,0) * core->getDeltaTime().asSeconds());
		moveFlag = true;
	}
	
	playerTexture.setSmooth(!moveFlag);

	
}

int main()
{
	AshCore engine(640, 480, 120, "AshEngineWindow");
	engine.setEventHandlingFunction(eventHandlingFunction);

	engine.addScript("scene", "player", testScript);
	engine.loadScene("testScene.txt");
	
	engine.startEngine();
}