#include "GameEngine.h"
using namespace ash;
GameEngine::GameEngine(unsigned int width, unsigned int height, unsigned int fps)
{
	this->width = width;
	this->height = height;
	this->framePerSeconds = fps;
}
GameEngine::~GameEngine()
{
	delete scene;
	buferObject = nullptr;
}

void GameEngine::run()
{
	//это только проверка на то, чтоб при запуске окна загружалось хоть что-то;
	if (!isSceneReady) { std::cout << "ENGINE_ERROR: no loaded scene" << '\n'; return; }
	window.create(sf::VideoMode(width, height), "AshEngineWindow");
	window.setFramerateLimit(framePerSeconds);
	window.setKeyRepeatEnabled(false);

	//main game loop
	sf::Clock clock;
	while (window.isOpen())
	{
		if (isSceneReady)
		{
			deltaTime = clock.restart();
			eventHandling();
			update(); 
			render(); //render
		}
	}
}

void GameEngine::eventHandling()
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

void GameEngine::update()
{

	targetCollions();

	//update camera position (target of player)
	if (dynamicCamera) // only if dynamic Camera is true;
	{
		ash::GameObject* player = nullptr;
		if (this->getObject("player", player))
		{
			sf::Vector2f playerPosition = player->getPosition();
			ash::Sizef playerSize = player->getSize();
			camera.setCenter(sf::Vector2f(playerPosition.x + (playerSize.width / 2), playerPosition.y + (playerSize.height / 2)));
		}
		player = nullptr;
	}
	//update animations on scene
	for (auto lay = scene->begin(); lay < scene->end(); ++lay) 
	{
		lay->sceneAnimationsUpdate(deltaTime);
	}

	//events
	while (!gameEventQueue.empty())
	{
		std::pair<int, ash::GameObject*>& sender = gameEventQueue.front();
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
			savePreviousPosition(buferObject);
			script.second(this, buferObject);
			buferObject = nullptr;
		}
	}
}

void GameEngine::render()
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

void GameEngine::playerInputStandart(sf::Keyboard::Key key, bool isPressed)
{
	return;
}

bool GameEngine::loadScene(std::string path)
{
	std::ifstream sceneFile(path);
	if (!sceneFile.is_open()) { return false; }
	if (scene != nullptr) { delete scene; }
	isSceneReady = false;
	scene = new std::vector<ash::GameLayout>;
	ash::GameObject newObj;
	std::string key;
	std::string value;
	std::map<std::string, std::string> propertiesBuffer;
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
			ash::GameLayout newLay;
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
			newObj.setPreviosPosition(newObj.getPosition());
		}
		else if (key == "url:")
		{
			sceneFile >> value;
			newObj.loadTexture(value);
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
		else if (key == "properties_set:")
		{
			if (!propertiesBuffer.empty()) { propertiesBuffer.clear(); }
			sceneFile >> value;
			propertiesBuffer = getFinishedProperties(value);
			if (!propertiesBuffer.empty())
			{
				newObj.setPropertiesSet(propertiesBuffer);
			}
		}
		else if (key == "collision:")
		{
			sceneFile >> value;
			if (value == "true")
			{
				newObj.enableCollision();
			}
			else if (value == "false")
			{
				newObj.disableCollision();
			}
			else
			{
				std::cout << "SCENE_ERROR: incorrect value of key <<  " << key << '\n';
				return false;
			}
		}
		else if (key == "collision_height:")
		{
			sceneFile >> value;
			newObj.setCollisionSize(0, std::stof(value));
		}
		else if (key == "collision_width:")
		{
			sceneFile >> value;
			newObj.setCollisionSize(std::stof(value), newObj.getCollisionSize().height);
		}
		else if (key == "collision_x:")
		{
			sceneFile >> value;
			newObj.setCollisionCenter(std::stof(value),0);
		}
		else if (key == "collision_y:")
		{
			sceneFile >> value;
			newObj.setCollisionCenter(newObj.getCollisionCenter().x, std::stof(value));
		}
		else if (key == "type:")
		{
			sceneFile >> value;
			if (value == "dynamic")
			{
				newObj.setVisible(true);
				(*scene)[layIndex].addObject(newObj.getName(), newObj, ash::objectType::dynamicType);
				newObj.clean();
			}
			else if(value == "static")
			{
				newObj.setVisible(true);
				(*scene)[layIndex].addObject(newObj.getName(), newObj, ash::objectType::staticType);
				newObj.clean();
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

void GameEngine::addSceneLay()
{
	ash::GameLayout newLay;
	scene->push_back(newLay);
}

void GameEngine::addObjectOnScene(ash::GameObject object, int objectType, int lay)
{
	ash::GameLayout& actualLay = (*scene)[lay];
	actualLay.addObject(object.getName(),object,objectType);
}

std::vector<ash::GameLayout>*& GameEngine::getActualScene()
{
	return scene;
}

void GameEngine::addPropertiesSetsConfig(const std::string& path)
{
	propertiesSetsConfigPath = path;
}

std::map<std::string, std::string> GameEngine::getFinishedProperties(const std::string& name)
{
	std::map<std::string, std::string> ans;
	if (propertiesSetsConfigPath.length() == 0)
	{
		std::cout << "PROPERTY ERROR<" << name << ">: no added config" << std::endl;
		return ans;
	}
	std::ifstream config(propertiesSetsConfigPath);
	if (!config.is_open())
	{
		std::cout << "PROPERTY ERROR<" << name << ">: incorrect file path" << std::endl;
		return ans;
	}
	std::string key;
	std::string value;
	int propertiesCount;
	while (!config.eof())
	{
		config >> key;
		if (key != name + ':')
		{
			continue;
		}
		config >> value;
		propertiesCount = std::stoi(value);
		for (int i = 0; i < propertiesCount; ++i)
		{
			config >> key;
			config >> value;
			key.pop_back();
			ans.insert(std::pair<std::string,std::string>(key,value));
		}
	}
	if (ans.empty())
	{
		std::cout << "PROPERTY ERROR<" << name << ">: incorrect name of set" << std::endl;
	}
	return ans;
}

void ash::GameEngine::targetCollions()
{
	sf::Vector2f posBuffer;
	
	for (int i = 0; i < scene->size(); ++i)
	{
		GameLayout& actualLay = (*scene)[i];
		std::map<std::string, GameObject>& objects = actualLay.getDynamicObjects();
		if (objects.size() == 0) { continue; }
		for (auto& elemet : objects)
		{
			GameObject& obj = elemet.second;
			if (!obj.isCollision()) { continue; }
			for (auto& elemetB : objects)
			{
				if (&elemetB == &elemet) { continue; }
				if (!elemetB.second.isCollision()) { continue; }

				GameObject& objB = elemetB.second;

				sf::FloatRect objBounds = sf::FloatRect(obj.getPosition().x + obj.getCollisionCenter().x, obj.getPosition().y + obj.getCollisionCenter().y, obj.getCollisionSize().width, obj.getCollisionSize().height);
				sf::FloatRect objBBounds = sf::FloatRect(objB.getPosition().x + objB.getCollisionCenter().x, objB.getPosition().y + objB.getCollisionCenter().y, objB.getCollisionSize().width, objB.getCollisionSize().height);
				
				if (objBounds.intersects(objBBounds))
				{
					posBuffer = obj.getPreviosPosition();
					obj.setPosition(posBuffer);
					obj.setPreviosPosition(posBuffer);
					
					posBuffer = objB.getPreviosPosition();
					objB.setPosition(posBuffer);
					objB.setPreviosPosition(posBuffer);
				}
			}
		}
	}
}

void ash::GameEngine::savePreviousPosition(GameObject*& obj)
{
	if (obj->isMoving())
	{
		obj->setPreviosPosition(obj->getPosition());
	}
}

bool GameEngine::getObject(std::string name, ash::GameObject*& buffer)
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

void GameEngine::addScript(std::string sceneName,std::string objectName, script scriptPtr)
{
	std::vector<std::pair<std::string,script>>& actualScripts = scripts[sceneName];
	actualScripts.push_back(std::pair<std::string, script>(objectName, scriptPtr));
}

void GameEngine::setPlayerInput(inputMethod method)
{
	std::pair<int, inputMethod> newInput(0, method);
	this->input = newInput;
	this->isCustomInputLoaded = true;
}

void GameEngine::addInstruction(int id, instruction instructionPtr)
{
	instructions.insert(std::pair<int, instruction>(id, instructionPtr));
}

void GameEngine::emitGameEvent(int eventId, ash::GameObject* sender)
{
	gameEventQueue.push(std::pair<int, ash::GameObject*>(eventId, sender));
}

sf::Vector2u GameEngine::getWindowSize()
{
	return sf::Vector2u(width,height);
}

float GameEngine::getDeltaTime()
{
	return this->deltaTime.asSeconds();
}

sf::View& GameEngine::getCamera()
{
	return this->camera;
}
