#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ShLlib/GameScene.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<map>

#define DELTA_TIME GAME->getDeltaTime()
#define OBJECT GAME->getVessel()
#define GAME_POINTER GameClass* GAME

class GameClass
{
	using script = void(*)(GameClass*);
private:
	//processing fields
	sf::Clock clock;
	sf::Time timeSinceLastUodate = sf::Time::Zero;
	sf::Time fixedDeltaTime = sf::Time::Zero;
	int framePerSeconds = 60;

	sf::Event actualEvent;
	sf::Keyboard::Key key;

	sf::RenderWindow window;
	sf::View camera;

	Game::GameObject* buferObject = nullptr;

	//game scene
	std::vector<Game::GameScene>* scene = nullptr;
	std::string sceneName = "";
	bool dynamicCamera = false;
	bool isSceneReady = false;

	unsigned int width;
	unsigned int height;
	//game scripts
	std::map<std::string, std::vector<script>> scripts;

public:
//work staff
	void run();
	void eventHandling();
	void update();
	void render();
	void playerInput(sf::Keyboard::Key,bool isPressed);
	bool loadScene(std::string path);

	bool getObject(std::string name, Game::GameObject*& buffer);
	void addScript(std::string sceneName, script scriptPtr);

	Game::GameObject*& getVessel();
	sf::Vector2u getWindowSize();
	sf::Time getDeltaTime();
//important staff
	//constructors
	GameClass(unsigned int width, unsigned int height,unsigned int fps)
	{
		camera.setSize(sf::Vector2f(width,height));
		this->width = width;
		this->height = height;
		this->framePerSeconds = fps;
		this->fixedDeltaTime = sf::seconds(1.6 / fps);
	}
	//destructors
	~GameClass() 
	{ 
		delete scene;
		buferObject = nullptr;
	}
};

