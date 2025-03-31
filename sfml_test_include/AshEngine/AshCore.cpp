#include "AshCore.h"
using namespace ash;
void ash::AshCore::run()
{
	mainWindow.create(sf::VideoMode(windowWidth, windowHeight), windowTitle);
	mainWindow.setFramerateLimit(framePerSeconds);
	mainWindow.setKeyRepeatEnabled(false);


	if (fullscreen)
	{
		camera.setSize(windowWidth, windowHeight);
	}
	mainWindow.setView(camera);

	sf::Clock clock;


	//game loop
	while (mainWindow.isOpen())
	{
		deltaTime = clock.restart();
		if(evHandlingFunction != nullptr)
		{
			evHandlingFunction(*this);
		}
		else { eventHandlingStandart(); }
		update();
		render();

	}
}

void ash::AshCore::eventHandlingStandart()
{
	if (mainWindow.pollEvent(actualEvent))
	{
		switch (actualEvent.type)
		{
		case sf::Event::Closed: {mainWindow.close(); } break;
		default:
			break;
		}
	}
}

void ash::AshCore::update()
{
	if (dynamicCamera)
	{
		AshEntity& player = getEntity("player");
		camera.setCenter(player.getGlobalBounds().getPosition());
	}
	targetCollions();
	processSignals();
	updateTextures();
	updateEntity();
}

void ash::AshCore::render()
{
	mainWindow.clear();
	mainWindow.setView(camera);

	if (actualScene != nullptr)
	{
		for (auto& lay : *actualScene)
		{
			for (auto& element : lay.second)
			{
				AshEntity& entity = element.second;
				if (!entity.isVisible() or !entity.isDrawable())
				{
					continue;
				}
				mainWindow.draw(entity);
			}
		}
	}

	mainWindow.display();
}

void ash::AshCore::updateTextures()
{
	if (actualScene == nullptr) { return; }

	//preparing
	updateCameraBounds();
	std::map<std::string, std::pair<sf::Texture*, AshResourceManager::textureSettings>>& loadedTextures = resourceManager.getMapOfLoadedTextures();
	for (auto& texture : loadedTextures){texture.second.second.countOfVisibleEntity = 0;}

	//process
	for (auto& lay : *actualScene)
	{
		for (auto& elementOfLay : lay.second)
		{
			AshEntity& entity = elementOfLay.second;
			if (camBounds.intersects(entity.getGlobalBounds()))
			{
				if (!entity.isDrawable())
				{
					entity.setDrawable(true);
					entity.setTexture(resourceManager.getTexture(entity.getTexturePath()));
				}
				AshResourceManager::textureSettings& settingToUpdate = loadedTextures[entity.getTexturePath()].second;
				sf::Texture& textureWithFreshSettings = *loadedTextures[entity.getTexturePath()].first;

				++loadedTextures[entity.getTexturePath()].second.countOfVisibleEntity;
				settingToUpdate.updateTextureSettings(textureWithFreshSettings);
				continue;
			}
			//the way, when entitity isn't in camera view
			if (entity.isDrawable()) {entity.setDrawable(false);} //if entity was in camera view on previous iteration of the game loop
		}
	}
	for (auto& texture : loadedTextures)
	{
		if (texture.second.second.countOfVisibleEntity == 0) { resourceManager.dropTexture(texture.first); }
	}
}

void ash::AshCore::updateCameraBounds()
{
	camPosition = sf::Vector2f(camCenter.x - camSize.x / 2, camCenter.y - camSize.y / 2);
	camBounds = sf::FloatRect(camPosition, camSize);
}

void ash::AshCore::processSignals()
{
	auto slotsBaseIter = slotsBase.find(actualSceneName);
	if (slotsBaseIter == slotsBase.end()) { return; }

	std::map<int, script>& actualSlotsBase = (*slotsBaseIter).second;
	while (!signalsQueue.empty())
	{
		std::pair<int, AshEntity&>& sender = signalsQueue.front();
		auto slotIter = actualSlotsBase.find(sender.first);
		if (slotIter == actualSlotsBase.end()) { continue; }
		(*slotIter).second(this, sender.second);
		signalsQueue.pop();
	}
}

void ash::AshCore::updateEntity()
{
	bool areScriptsExist = false;
	auto scriptsBaseIter = scriptsBase.find(actualSceneName);
	if (scriptsBaseIter != scriptsBase.end()) { areScriptsExist = true; }
	for (auto& lay : *actualScene)
	{
		for (auto& element : lay.second)
		{
			AshEntity& entity = element.second;
			entity.setPreviousPosition(entity.getPosition());
			if (areScriptsExist)
			{
				auto scriptIter = (*scriptsBaseIter).second.find(entity.getName());
				if (scriptIter != (*scriptsBaseIter).second.end()) { (*scriptIter).second(this, entity); }

			}

			animator.updateAnimation(deltaTime, entity);
		}
	}
}

void ash::AshCore::parsProperties(const std::string& propertyName, AshEntity& entity)
{
	std::ifstream propertieFile(properites);
	std::string buffer;
	while (!propertieFile.eof())
	{
		propertieFile >> buffer;
		if (buffer == propertyName + ':')
		{
			int count;
			propertieFile >> count;
			for (int i = 0; i < count; ++i)
			{
				propertieFile >> buffer; 
				if (buffer == "bool")
				{
					propertieFile >> buffer;
					entity.addProperty(p_bool, buffer, false);
					bool value; propertieFile >> value;
					entity.getBool(buffer) = value;
				}
				else if (buffer == "int")
				{
					propertieFile >> buffer;
					entity.addProperty(p_bool, buffer, 0);
					int value; propertieFile >> value;
					entity.getInt(buffer) = value;
				}
				else if (buffer == "float")
				{
					propertieFile >> buffer;
					entity.addProperty(p_bool, buffer, 0.f);
					float value; propertieFile >> value;
					entity.getFloat(buffer) = value;
				}
				else
				{
					std::cout << "CORE ERROR: property <" << propertyName << "> incorrect property type: " << buffer << std::endl;
					propertieFile.close();
					return;
				}
			}
		}
	}

	propertieFile.close();
}

void ash::AshCore::targetCollions()
{
	sf::Vector2f posBuffer;

	for (int i = 0; i < actualScene->size(); ++i)
	{
		std::map<std::string, AshEntity>& actualLay = (*actualScene)[i];
		if (actualLay.size() == 0) { continue; }
		for (auto& elemet : actualLay)
		{
			AshEntity& entity = elemet.second;
			if (!entity.isColliding()) { continue; }
			for (auto& elemetB : actualLay)
			{
				if (&elemetB == &elemet) { continue; }
				if (!elemetB.second.isColliding()) { continue; }

				AshEntity& entityB = elemetB.second;

				//todo коллизия сама не скейлится. Т.е. Если я укажу спрайту scale = 2, то я должен руками заскейлить коллизию тоже. Это проблема
				sf::FloatRect objBounds = sf::FloatRect(entity.getPosition().x + entity.getCollisionPosition().x, entity.getPosition().y + entity.getCollisionPosition().y, entity.getCollisionSize().x, entity.getCollisionSize().y);
				sf::FloatRect objBBounds = sf::FloatRect(entityB.getPosition().x + entityB.getCollisionPosition().x, entityB.getPosition().y + entityB.getCollisionPosition().y, entityB.getCollisionSize().x, entityB.getCollisionSize().y);

				if (objBounds.intersects(objBBounds))
				{
					posBuffer = entity.getPreviousPosition();
					entity.setPosition(posBuffer);
					entity.setPreviousPosition(posBuffer);

					posBuffer = entityB.getPreviousPosition();
					entityB.setPosition(posBuffer);
					entityB.setPreviousPosition(posBuffer);
				}
			}
		}
	}
}

ash::AshCore::AshCore(const unsigned int& width, const int& height, const unsigned int& fps, const std::string& windowTitle)
{
	framePerSeconds = fps;
	windowWidth = width;
	windowHeight = height;
	this->windowTitle = windowTitle;
}

AshEntity& ash::AshCore::getEntity(const std::string& name)
{
	
	AshEntity emptyObject; //todo подумать как избавиться от этого
	if (actualScene == nullptr) 
	{
		std::cout << "CORE ERROR: no loaded scene\n";
		return emptyObject; 
	}
	for (auto& lay : *actualScene)
	{
		std::map<std::string, AshEntity>& actualLay = lay.second;
		auto entityIter = actualLay.find(name);
		if (entityIter != actualLay.end())
		{
			return (*entityIter).second;
		}
	}
	std::cout << "CORE ERROR: no entity <" + name + ">\n";
	return emptyObject;
}

void ash::AshCore::emitSignal(const int& signalID,  AshEntity&  entity)
{
	signalsQueue.push(std::pair<int, AshEntity&>(signalID, entity));
}

void ash::AshCore::loadScene(const std::string& sceneName)
{
	
	std::ifstream sceneFile(sceneDir + sceneName);
	if (!sceneFile.is_open())
	{
		std::cout << "CORE ERROR: no scene <" + sceneName + ">\n";
		return;
	}
	AshEntity entityBuffer;
	AshAnimator::animation animationBuffer; 
	AshResourceManager::textureSettings textureBuffer;

	std::string key;
	std::string value;
	int layIndex = -1;

	processSignals();

	if (actualScene != nullptr) { delete actualScene; actualScene = nullptr; }
	actualScene = new sceneType;

	while (!sceneFile.eof())
	{
		sceneFile >> key;
		if (key == "scene_name:")
		{
			sceneFile >> value;
			actualSceneName = value;
		}
		else if (key == "camera_type:")
		{
			sceneFile >> value;
			if (value == "dynamic") { dynamicCamera = true; }
			else if (value == "static") { dynamicCamera = false; }
			else
			{
				std::cout << "CORE ERROR: incorrect value of key <" + key + ">\n";
				return;
			}
		}
		else if (key == "camera_fullscreen:")
		{
			sceneFile >> value;
			if (value == "true") { fullscreen = true; }
			else if (value == "false") { fullscreen = false; }
			else
			{
				std::cout << "CORE ERROR: incorrect value of key <" + key + ">\n";
				return;
			}
		}
		else if (key == "camera_x:")
		{
			sceneFile >> value;
			camera.setCenter(std::stof(value), 0);
		}
		else if (key == "camera_y:")
		{
			sceneFile >> value;
			camera.setCenter(camera.getCenter().x, std::stof(value));
		}
		else if (key == "camera_width:")
		{
			sceneFile >> value;
			camera.setSize(std::stof(value), 0);
		}
		else if (key == "camera_height:")
		{
			sceneFile >> value;
			camera.setSize(camera.getSize().x, std::stof(value));
		}
		else if (key == "lay")
		{
			++layIndex;
			std::pair<int, std::map<std::string, AshEntity>> pair;
			pair.first = layIndex;
			actualScene->insert(pair);
		}
		else if (key == "name:")
		{
			sceneFile >> value;
			entityBuffer.setName(value);
		}
		else if (key == "ent_x:")
		{
			sceneFile >> value;
			entityBuffer.setPosition(std::stof(value), 0);
		}
		else if (key == "ent_y:")
		{
			sceneFile >> value;
			entityBuffer.setPosition(entityBuffer.getPosition().x, std::stof(value));
		}
		else if (key == "ent_width:")
		{
			sceneFile >> value;
			entityBuffer.setTextureRect(sf::IntRect(0,0,std::stof(value), 0));
		}
		else if (key == "ent_heigth:")
		{
			sceneFile >> value;
			entityBuffer.setTextureRect(sf::IntRect(0, 0, entityBuffer.getTextureRect().width, std::stof(value)));
		}
		else if (key == "ent_scale_x:")
		{
			sceneFile >> value;
			entityBuffer.setScale(std::stof(value), 1);
		}
		else if (key == "ent_scale_y:")
		{
			sceneFile >> value;
			entityBuffer.setScale(entityBuffer.getScale().x, std::stof(value));
		}
		else if (key == "url:")
		{
			sceneFile >> value;
			entityBuffer.setTexturePath(value);       
			resourceManager.loadTextureFromDir(value);
		}
		else if (key == "visible:")
		{
			sceneFile >> value;
			if (value == "true") { entityBuffer.setVisible(true); }
			else if (value == "false") { entityBuffer.setVisible(false); }
			else
			{
				std::cout << "CORE ERROR: incorrect value of key <" + key + ">\n";
				return;
			}
		}
		else if (key == "repeated:")
		{
			sceneFile >> value;
			if (value == "true") { resourceManager.getSettings(entityBuffer.getTexturePath()).repeated = true; }
			else if (value == "false") { resourceManager.getSettings(entityBuffer.getTexturePath()).repeated = false; }
			else
			{
				std::cout << "CORE ERROR: incorrect value of key <" + key + ">\n";
				return;
			}

		}
		else if (key == "collision:")
		{
			sceneFile >> value;
			if (value == "true") { entityBuffer.setColliding(true); }
			else if (value == "false") { entityBuffer.setColliding(false); }
			else
			{
				std::cout << "CORE ERROR: incorrect value of key <" + key + ">\n";
				return;
			}
		}
		else if (key == "col_x:")
		{
			sceneFile >> value;
			entityBuffer.setCollisionPosition(sf::Vector2f(std::stof(value), 0));
		}
		else if (key == "col_y:")
		{
			sceneFile >> value;
			entityBuffer.setCollisionPosition(sf::Vector2f(entityBuffer.getCollisionPosition().x, std::stof(value)));
		}
		else if (key == "col_width:")
		{
			sceneFile >> value;
			entityBuffer.setCollisionSize(sf::Vector2f(std::stof(value), 0));
		}
		else if (key == "col_heigth:")
		{
			sceneFile >> value;
			entityBuffer.setCollisionSize(sf::Vector2f(entityBuffer.getCollisionSize().x, std::stof(value)));
			}
		else if (key == "animated:")
		{
			sceneFile >> value;
			if (value == "true")
			{
				animationBuffer.enable = true;
				animator.addAnimation(entityBuffer.getTexturePath(), animationBuffer);
			}
			else
			{
				animationBuffer.enable = false;
			}
		}
		else if (key == "frame_count:")
		{
			sceneFile >> value;
			if (!animationBuffer.enable) { continue; }
			animator.getAnimation(entityBuffer.getTexturePath()).frameCount = std::stoi(value);
		}
		else if (key == "fps:")
		{
			sceneFile >> value;
			if (!animationBuffer.enable) { continue; }
			animator.getAnimation(entityBuffer.getTexturePath()).framePerSeconds = std::stoi(value);
		}
		else if (key == "properties:")
		{
			sceneFile >> value;
			parsProperties(value, entityBuffer);
		}
		else if (key == "end.")
		{
			std::map<std::string, AshEntity>& actualLay = (*actualScene)[layIndex];
			actualLay.insert(std::pair<std::string,AshEntity>(entityBuffer.getName(),entityBuffer));
			entityBuffer.setToDefault();
			animationBuffer.clear();
		}
		else
		{
			std::cout << "CORE ERROR: incorrect key: " + key << std::endl;
			delete actualScene;
			actualScene = nullptr;
			return;
		}
	}
	sceneReady = true;
}

void ash::AshCore::pushEntity(const AshEntity& entity, const int& lay)
{
	(*actualScene)[lay][entity.getName()] = entity;
}

void ash::AshCore::pushEntity(const AshEntity& entity, const std::string& entityName, const int& lay)
{
	(*actualScene)[lay][entityName] = entity;
}

void ash::AshCore::addEntities(const std::map<std::string, AshEntity>& entitiesMap, const int& lay)
{
	std::map<std::string, AshEntity>& layToEdit = (*actualScene)[lay];
	for (auto& entity : entitiesMap)
	{
		layToEdit[entity.first] = entity.second;
	}
}

void ash::AshCore::popEntity(const std::string& entitynName, const int& lay)
{
	(*actualScene)[lay].erase(entitynName); //нужна проверка, существеь ли объект
}

void ash::AshCore::addScript(const std::string& sceneName, const std::string& entityName, script yourScript)
{
	scriptsBase[sceneName][entityName] = yourScript;	
}

void ash::AshCore::addSlot(const std::string& sceneName, const int& signalID, script yourSlot)
{
	slotsBase[sceneName][signalID] = yourSlot;
}
