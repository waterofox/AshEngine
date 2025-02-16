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

	updateTextures();
	
}

void ash::AshCore::render()
{
	mainWindow.clear();

	if (actualScene != nullptr)
	{
		for (auto& lay : *actualScene)
		{
			for (auto& element : lay.second)
			{
				AshEntity& entity = element.second;
				if (!entity.isVisible())
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

	sf::FloatRect cameraRect;
	sf::FloatRect entityBounds;
	cameraRect.width = camera.getSize().x;
	cameraRect.height = camera.getSize().y;
	cameraRect.left = camera.getCenter().x - camera.getSize().x / 2;
	cameraRect.top = camera.getCenter().y - camera.getSize().y / 2;


	for (auto& lay : *actualScene)
	{
		for (auto& element : lay.second)
		{
			AshEntity& entity = element.second;
			entityBounds = entity.getGlobalBounds();
			if (!cameraRect.intersects(entityBounds))
			{
				entity.setDrawable(false);
				resourceManager.dropTexture(entity.getTexture());
			}
			else
			{
				if (!entity.isDrawable())
				{
					entity.setDrawable(true);
					entity.setTexture(resourceManager.getTexture(entity.getTexturePath()));
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

void ash::AshCore::loadScene(const std::string& sceneName)
{
	std::ifstream sceneFile(sceneDir + sceneName);
	if (!sceneFile.is_open())
	{
		std::cout << "CORE ERROR: no scene <" + sceneName + ">\n";
		return;
	}
	AshEntity entityBuffer;
	std::string key;
	std::string value;
	int layIndex = -1;

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
		else if (key == "ent_height:")
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
		else if (key == "end.")
		{
			std::map<std::string, AshEntity>& actualLay = (*actualScene)[layIndex];
			actualLay.insert(std::pair<std::string,AshEntity>(entityBuffer.getName(),entityBuffer));
			entityBuffer.setToDefault();
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
