#include "GameClass.h"

void GameClass::run()
{
	window.create(sf::VideoMode(640, 480), "GameClassWindow");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	while (window.isOpen())
	{
		eventHandling();
		timeSinceLastUodate += clock.restart();
		while (timeSinceLastUodate > fixedDeltaTime)
		{
			timeSinceLastUodate -= fixedDeltaTime;
			eventHandling();
			update();
		}
		render();
	}
}

void GameClass::eventHandling()
{
	if (window.pollEvent(actualEvent))
	{

		switch (actualEvent.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed: { playerInput(actualEvent.key.code, true); }break;
		case sf::Event::KeyReleased: {playerInput(actualEvent.key.code, false); }break;
		default:
			break;
		}
	}
}

void GameClass::update()
{
	sf::Vector2f playerPosition = scene->getObject("player").getPosition();
	Game::Sizef playerSize = scene->getObject("player").getSize();

	camera.setCenter(sf::Vector2f(playerPosition.x + (playerSize.width/2),playerPosition.y + (playerSize.height/2)));

	scene->sceneAnimationsUpdate(fixedDeltaTime);
	//вызов скриптов
	controlScript(scene->getObject("player"));
}

void GameClass::render()
{
	//рендер
	window.clear();
	window.setView(camera);
	scene->renderScene(window);
	window.display();
}

void GameClass::playerInput(sf::Keyboard::Key key, bool isPressed)
{
	Game::GameObject& player = scene->getObject("player");
	switch (key)
	{
	case sf::Keyboard::D: {player.moveRight = isPressed; }break;
	case sf::Keyboard::S: {player.moveDown = isPressed; }break;
	case sf::Keyboard::A: {player.moveLeft = isPressed; }break;
	case sf::Keyboard::W: {player.moveUp = isPressed; }break;
	default:
		break;
	}
}

bool GameClass::loadScene(std::string path)
{
	std::ifstream sceneFile(path);
	if (!sceneFile.is_open()) { return false; }

	float fBuffer;
	float fBuffer2;
	std::string strBuffer;

	Game::GameObject newObj;
	while (!sceneFile.eof())
	{
		//cords
		sceneFile >> fBuffer;
		newObj.setPosition(sf::Vector2f(fBuffer, 0));
		sceneFile >> fBuffer;
		newObj.moveY(fBuffer);
		//scale
		sceneFile >> fBuffer;
		sceneFile >> fBuffer2;
		newObj.setScale(sf::Vector2f(fBuffer,fBuffer2));
		//texture
		sceneFile >> strBuffer;
		newObj.updateTexture(strBuffer);
		//add
		sceneFile >> strBuffer;
		if (strBuffer == "dyn")
		{
			sceneFile >> strBuffer;
			scene->addObject(strBuffer, newObj, Game::objectType::dynamicType);
		}
		else
		{
			sceneFile >> strBuffer;
			scene->addObject(strBuffer, newObj, Game::objectType::staticType);
		}
	}
	sceneFile.close();
	return true;
}
