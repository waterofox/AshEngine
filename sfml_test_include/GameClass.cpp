#include "GameClass.h"
void GameClass::run()
{
	window.create(sf::VideoMode(640, 480), "GameClassWindow");
	window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(true);
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
	Game::GameObject* player = nullptr;
	this->getObject("player", player);
	sf::Vector2f playerPosition = player->getPosition();
	Game::Sizef playerSize = player->getSize();

	camera.setCenter(sf::Vector2f(playerPosition.x + (playerSize.width/2),playerPosition.y + (playerSize.height/2)));

	for (auto lay = scene->begin(); lay < scene->end(); ++lay)
	{
		lay->sceneAnimationsUpdate(fixedDeltaTime);
	}
	//вызов скриптов
	controlScript();
}

void GameClass::render()
{
	//рендер
	window.clear();
	window.setView(camera);
	for (auto lay = scene->begin(); lay < scene->end(); ++lay)
	{
		lay->renderScene(window);
	}
	window.display();
}

void GameClass::playerInput(sf::Keyboard::Key key, bool isPressed)
{
	Game::GameObject* player = nullptr;
	this->getObject("player", player);
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

bool GameClass::loadScene(std::string path)
{
	std::ifstream sceneFile(path);
	if (!sceneFile.is_open()) { return false; }
	if (scene != nullptr) { delete scene; }
	scene = new std::vector<Game::GameScene>;
	Game::GameObject newObj;
	std::string key;
	std::string value;
	int layIndex = -1;
	
	while (!sceneFile.eof())
	{
		sceneFile >> key;
		if (key == "view_status:")
		{
			sceneFile >> value;
			if (value == "dynamic") {} //todo set
			else if (value == "static") {}//todo set
			else {}//todo error output
		}
		else if (key == "lay")
		{
			++layIndex;
			Game::GameScene newLay;
			scene->push_back(newLay);
		}
		else if (key == "name:")
		{
			sceneFile >> value;
			newObj.setName(value);
		}
		else if (key == "X:")
		{
			sceneFile >> value;
			newObj.setX(std::stof(value));
		}
		else if (key == "Y:")
		{
			sceneFile >> value;
			newObj.setY(std::stof(value));
		}
		else if (key == "scale_x:")
		{
			sceneFile >> value;
			newObj.setScale(sf::Vector2f(std::stof(value), 0));
		}
		else if (key == "scale_y:")
		{
			sceneFile >> value;
			newObj.setScale(sf::Vector2f(newObj.getSFMlobj().getScale().x , std::stof(value)));
		}
		else if (key == "url:")
		{
			sceneFile >> value;
			newObj.updateTexture(value);
		}
		else if (key == "animated:")
		{
			sceneFile >> value;
			if (value == "true") { newObj.enableAnimation(); }
			else { newObj.disableAnimation(); }
		}
		else if (key == "fps:")
		{
			sceneFile >> value;
			newObj.setFramePerSeconds(std::stoi(value));
		}
		else if (key == "type:")
		{
			sceneFile >> value;
			if (value == "dynamic")
			{
				(*scene)[layIndex].addObject(newObj.gertName(), newObj, Game::objectType::dynamicType);
			}
			else
			{
				(*scene)[layIndex].addObject(newObj.gertName(), newObj, Game::objectType::staticType);
			}
		}		
	}

	sceneFile.close();
	return true;

}

bool GameClass::getObject(std::string name, Game::GameObject*& buffer)
{
	for (auto lay = scene->begin(); lay < scene->end(); ++lay)
	{
		if (lay->getObject(name, buffer))
		{
			return true;
		}
	}
	return false;
}
