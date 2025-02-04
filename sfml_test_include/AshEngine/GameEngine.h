#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameLayout.h"

#include <iostream>
#include <fstream>

#include<vector>
#include<map>
#include<queue>

namespace ash
{
	#define DELTA_TIME Game->getDeltaTime()
	//some types
	class GameEngine
	{
		//type for game's scripts:
		using script = void(*)(GameEngine*, ash::GameObject*);

		//type for event's instructions
		using instruction = void(*)(GameEngine*, ash::GameObject*);

		//type for player input method:
		using inputMethod = void(*)(GameEngine*, sf::Keyboard::Key, bool);
	private:

		//time
		sf::Time deltaTime = sf::Time::Zero;

		//frames
		sf::RenderWindow window; //main window
		unsigned int width;
		unsigned int height;

		//system evets 
		sf::Event actualEvent;
		sf::Keyboard::Key key;
		std::pair<int, inputMethod> input; //for custom player input (this is a ********, i should fix that in 2025)
		bool isCustomInputLoaded = false;

		//game events 
		std::map<int, instruction> instructions;
		std::queue<std::pair<int, ash::GameObject*>> gameEventQueue;		//todo think about this staff before write "setScene" method

		//game
		int framePerSeconds = 60;
		std::string propertiesSetsConfigPath;

		//game scene
		std::vector<ash::GameLayout>* scene = nullptr;
		std::string sceneName = "";
		bool isSceneReady = false;

		//camera
		sf::View camera;
		bool dynamicCamera = false;
		bool fullscreen = false;

		ash::GameObject* buferObject = nullptr; // vessel for objects from scene

		//game scripts
		std::map<std::string, std::vector<std::pair<std::string, script>>> scripts;

		//process methods
		void run();
		void eventHandling();
		void update();
		void render();
		void playerInputStandart(sf::Keyboard::Key, bool isPressed); //standart input (it is an empty)
		std::map<std::string, std::string> getFinishedProperties(const std::string& name);

	public:

		//constructors
		GameEngine(unsigned int width, unsigned int height, unsigned int fps);
		~GameEngine();

		//setting up the game
		void startGame() { run(); }
		void addPropertiesSetsConfig(const std::string& path);
		void addScript(std::string sceneName, std::string objectName, script scriptPtr);
		void setPlayerInput(inputMethod method);
		void addInstruction(int id, instruction instructionPtr);
		void emitGameEvent(int eventId, ash::GameObject* sender);


		//manipulating the scene
		bool loadScene(std::string path);
		void addSceneLay();
		void addObjectOnScene(ash::GameObject object, int objectType, int lay);
		std::vector<ash::GameLayout>*& getActualScene();
		bool getObject(std::string name, ash::GameObject*& buffer);

		//getters <time>
		float getDeltaTime();
		sf::Time getElapsedTime() { return deltaTime; }

		//getters <camera>
		sf::View& getCamera();

		//getters <window>
		sf::Vector2u getWindowSize();

	};
};


