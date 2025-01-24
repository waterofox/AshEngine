#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameScene.h"

#include <iostream>
#include <fstream>

#include<vector>
#include<map>
#include<queue>


//define for lazy person:
#define DELTA_TIME Game->getDeltaTime()
class GameClass
{
	//type for game's scripts:
	using script = void(*)(GameClass*,Game::GameObject*);

	//type for event's instructions
	using instruction = void(*)(GameClass*, Game::GameObject*);

	//type for player input method:
	using inputMethod = void(*)(GameClass*,sf::Keyboard::Key,bool);
private:
	//game tic fields:
	sf::Clock clock;
	sf::Time timeSinceLastUodate = sf::Time::Zero;
	sf::Time fixedDeltaTime = sf::Time::Zero;
	int framePerSeconds = 60;

	//system evets fields:
	sf::Event actualEvent;
	sf::Keyboard::Key key;
	std::pair<int, inputMethod> input; //for custom player input (this is a ********, i should fix that in 2025)
	bool isCustomInputLoaded = false; 

	//game events instructions
	std::map<int, instruction> instructions;
	std::queue<std::pair<int,Game::GameObject*>> gameEventQueue;		//todo think about this staff before write "setScene" method

	sf::RenderWindow window; //main window
	unsigned int width;
	unsigned int height;

	//game scene
	std::vector<Game::GameScene>* scene = nullptr;
	std::string sceneName = "";
	bool isSceneReady = false;

	sf::View camera; //camera
	bool dynamicCamera = false;
	bool fullscreen = false;

	Game::GameObject* buferObject = nullptr; // vessel for objects from scene

	//game scripts
	std::map<std::string, std::vector<std::pair<std::string, script>>> scripts;

public:
	//sfml classic methods
	void run();
	void eventHandling();
	void update();
	void render();

	void playerInputStandart(sf::Keyboard::Key, bool isPressed); //standart input (it is an empty)


	bool loadScene(std::string path);
	void addSceneLay();
	void addObjectonScene(Game::GameObject object,int objectType, int lay);
	std::vector<Game::GameScene>*& getActualScene();

	bool getObject(std::string name, Game::GameObject*& buffer);

	void addScript(std::string sceneName,std::string objectName, script scriptPtr);

	void setPlayerInput(inputMethod method);

	void addInstruction(int id, instruction instructionPtr);
	void emitGameEvent(int eventId,Game::GameObject * sender);

	sf::Vector2u getWindowSize();
	
	sf::Time getDeltaTime();

	sf::View& getCamera();


//important staff
	//constructors
	GameClass(unsigned int width, unsigned int height,unsigned int fps)
	{
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

