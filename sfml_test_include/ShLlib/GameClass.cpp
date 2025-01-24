#include "GameClass.h"
void GameClass::run()
{
	//это только проверка на то, чтоб при запуске окна загружалось хоть что-то;
	if (!isSceneReady) { std::cout << "ENGINE_ERROR: no loaded scene" << '\n'; return; }
	window.create(sf::VideoMode(width, height), "GameClassWindow");
	window.setFramerateLimit(framePerSeconds);

	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	//main game loop

	while (window.isOpen())
	{
		if (isSceneReady)
		{
			eventHandling();
			timeSinceLastUodate += clock.restart();
			while (timeSinceLastUodate > fixedDeltaTime)
			{
				timeSinceLastUodate -= fixedDeltaTime;
				eventHandling(); //keyboard/mouse event catch
				update(); // update gamelogic and prerender
			}
			render(); //render
		}
	}
}

void GameClass::eventHandling()
{
	if (window.pollEvent(actualEvent))
	{

		switch (actualEvent.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed:
		{
			if (isCustomInputLoaded)
			{
				input.second(this, actualEvent.key.code, true);
			}
			else
			{
				playerInputStandart(actualEvent.key.code, true);
			}
		}break;
		case sf::Event::KeyReleased:
		{
			if (isCustomInputLoaded)
			{
				input.second(this, actualEvent.key.code, false);
			}
			else
			{
				playerInputStandart(actualEvent.key.code, false);
			}
		default:
			break;
		}
		}
	}
}

void GameClass::update()
{
	//update camera position (target of player)
	if (dynamicCamera) // only if dynamicCamera is true;
	{
		Game::GameObject* player = nullptr;
		if (this->getObject("player", player))
		{
			sf::Vector2f playerPosition = player->getPosition();
			Game::Sizef playerSize = player->getSize();
			camera.setCenter(sf::Vector2f(playerPosition.x + (playerSize.width / 2), playerPosition.y + (playerSize.height / 2)));
		}
		player = nullptr;
	}
	//update animations on scene
	for (auto lay = scene->begin(); lay < scene->end(); ++lay) 
	{
		lay->sceneAnimationsUpdate(fixedDeltaTime);
	}

	//events
	while (!gameEventQueue.empty())
	{
		std::pair<int, Game::GameObject*>& sender = gameEventQueue.front();
		auto& instructionPtr = instructions[sender.first];
		instructionPtr(this, sender.second);
		gameEventQueue.pop();
	}

	//scripts
	std::vector<std::pair<std::string,script>>& actualScrits = scripts[sceneName];
	for (auto& script : actualScrits)
	{
		if (this->getObject(script.first, buferObject))
		{
			script.second(this, buferObject);
			buferObject = nullptr;
		}
	}
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

void GameClass::playerInputStandart(sf::Keyboard::Key key, bool isPressed)
{
	return;
}

bool GameClass::loadScene(std::string path)
{
	std::ifstream sceneFile(path);
	if (!sceneFile.is_open()) { return false; }
	if (scene != nullptr) { delete scene; }
	isSceneReady = false;
	scene = new std::vector<Game::GameScene>;
	Game::GameObject newObj;
	std::string key;
	std::string value;
	int layIndex = -1;


	while (!sceneFile.eof())
	{
		sceneFile >> key;
		if (key == "camera:")
		{
			sceneFile >> value;
			if (value == "dynamic") { dynamicCamera = true; } 
			else if (value == "static") { dynamicCamera = false; }
			else 
			{
				std::cout << "SCENE_ERROR: incorrect value of key <<  " << key << '\n';
				return false;
			}
		}
		else if(key == "fullscreen:")
		{
			sceneFile >> value;
			if (value == "true") { fullscreen = true; }
			else if (value == "false") { fullscreen = false; }
			else
			{
				std::cout << "SCENE_ERROR: incorrect value of key << " << key << '\n';
				return false;
			}
		}
		else if (key == "camera_width:")
		{
			sceneFile >> value;
			if (!fullscreen) { camera.setSize(std::stof(value), 0); }
			else { camera.setSize(width, height); }
		}
		else if (key == "camera_height:")
		{
			sceneFile >> value;
			if (!fullscreen) { camera.setSize(camera.getSize().x, std::stof(value)); }
			else { camera.setSize(width, height);}
		}
		else if (key == "camera_X:")
		{
			sceneFile >> value;
			if (!dynamicCamera) { camera.setCenter(std::stof(value), 0); }
		}
		else if (key == "camera_Y:")
		{
			sceneFile >> value;
			if (!dynamicCamera) { camera.setCenter(camera.getCenter().x, std::stof(value)); }
		}
		else if(key == "scene:")
		{
			sceneFile >> value;
			sceneName = value;
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
		else if (key == "url:")
		{
			sceneFile >> value;
			newObj.updateTexture(value);
			newObj.setTexturePath(value);
		}
		else if (key == "obj_height:")
		{
			sceneFile >> value;
			newObj.getSFMlobj().setTextureRect(sf::IntRect(0, 0, 0, std::stoi(value)));
		}
		else if (key == "obj_width:")
		{
			sceneFile >> value;
			newObj.getSFMlobj().setTextureRect(sf::IntRect(0, 0, std::stoi(value), newObj.getSFMlobj().getTextureRect().height));
		}
		else if (key == "scale_x:")
		{
			sceneFile >> value;
			newObj.setScale(sf::Vector2f(std::stof(value), 0));
		}
		else if (key == "scale_y:")
		{
			sceneFile >> value;
			newObj.setScale(sf::Vector2f(newObj.getSFMlobj().getScale().x, std::stof(value)));
		}
		else if (key == "repeated:")
		{
			sceneFile >> value;
			if (value == "true")
			{
				newObj.setTextureRepeat(true);
			}
			else if (value == "false") { newObj.setTextureRepeat(false); }
			else 
			{
				sceneFile.close();
				std::cout << "SCENE_ERROR: incorrect value of key <<  " << key << '\n';
				return false;
			}
		}
		else if (key == "visible:")
		{
			sceneFile >> value;
			if (value == "true") { newObj.setVisible(true); }
			else if (value == "false") { newObj.setVisible(false); }
			else
			{
				sceneFile.close();
				std::cout << "SCENE_ERROR: incorrect value of key <<  " << key << '\n';
				return false;
			}
		}
		else if (key == "animated:")
		{
			sceneFile >> value;
			if (value == "true") { newObj.enableAnimation(); }
			else if(value == "false"){ newObj.disableAnimation(); }
			else
			{
				sceneFile.close();
				std::cout << "SCENE_ERROR: incorrect value of key <<  " << key << '\n';
				return false;
			}
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
				(*scene)[layIndex].addObject(newObj.getName(), newObj, Game::objectType::dynamicType);
				newObj.setVisible(true);
			}
			else if(value == "static")
			{
				(*scene)[layIndex].addObject(newObj.getName(), newObj, Game::objectType::staticType);
				newObj.setVisible(true);
			}
			else
			{
				sceneFile.close();
				std::cout << "SCENE_ERROR: incorrect value of key <<  " << key << '\n';
				return false;
			}
		}
		else
		{
			sceneFile.close();
			std::cout << "SCENE_ERROR: incorrect key <<  " << key << '\n';
			return false;
		}
	}
	sceneFile.close();
	isSceneReady = true;
	return true;

}

void GameClass::addSceneLay()
{
	Game::GameScene newLay;
	scene->push_back(newLay);
}

void GameClass::addObjectonScene(Game::GameObject object, int objectType, int lay)
{
	Game::GameScene& actualLay = (*scene)[lay];
	actualLay.addObject(object.getName(),object,objectType);
}

std::vector<Game::GameScene>*& GameClass::getActualScene()
{
	return scene;
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

void GameClass::addScript(std::string sceneName,std::string objectName, script scriptPtr)
{
	std::vector<std::pair<std::string,script>>& actualScripts = scripts[sceneName];
	actualScripts.push_back(std::pair<std::string, script>(objectName, scriptPtr));
}

void GameClass::setPlayerInput(inputMethod method)
{
	std::pair<int, inputMethod> newInput(0, method);
	this->input = newInput;
	this->isCustomInputLoaded = true;
}

void GameClass::addInstruction(int id, instruction instructionPtr)
{
	instructions.insert(std::pair<int, instruction>(id, instructionPtr));
}

void GameClass::emitGameEvent(int eventId, Game::GameObject* sender)
{
	gameEventQueue.push(std::pair<int, Game::GameObject*>(eventId, sender));
}

sf::Vector2u GameClass::getWindowSize()
{
	return sf::Vector2u(width,height);
}

sf::Time GameClass::getDeltaTime()
{
	return this->fixedDeltaTime;
}

sf::View& GameClass::getCamera()
{
	return this->camera;
}
