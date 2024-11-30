#pragma once
#define FPS 60
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define DELTA_TIME sf::seconds(1.6 / FPS)

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ShLlib/GameScene.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<map>
class GameClass
{
	using script = void(*)(GameClass*);
private:
	//processing fields
	sf::Clock clock;
	sf::Time timeSinceLastUodate = sf::Time::Zero;
	sf::Time fixedDeltaTime = DELTA_TIME;

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
//important staff
	//constructors
	GameClass(){
		camera.setSize(sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT));
	}
	//destructors
	~GameClass() 
	{ 
		delete scene;
		delete buferObject;
	}
};

