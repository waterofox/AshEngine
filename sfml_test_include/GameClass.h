#pragma once
#define PLAYER_SPEED 150
#define FPS 60
#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "ShLlib/GameScene.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<map>
class GameClass
{
	using script = void(GameClass::*)();
private:
	//processing fields
	sf::Clock clock;
	sf::Time timeSinceLastUodate = sf::Time::Zero;
	sf::Time fixedDeltaTime = sf::seconds(1.6 / FPS);

	sf::Event actualEvent;
	sf::Keyboard::Key key;
	sf::RenderWindow window;
	sf::View camera;
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
//important staff
	//constructors
	GameClass(){
		isSceneReady =  loadScene("resources/scenes/testScene.txt");
		std::vector<script> jopa;
		jopa.push_back(&GameClass::controlScript);
		scripts.insert(std::pair<std::string, std::vector<script>>(sceneName, jopa));
		camera.setSize(sf::Vector2f(WINDOW_WIDTH,WINDOW_HEIGHT));
	}
	//destructors
	~GameClass() { delete scene; }

	//scripts
	void controlScript()
	{
		Game::GameObject* obj = nullptr;
		if (!this->getObject("player", obj)) {return;}
		if (obj->moveRight) { obj->moveX(PLAYER_SPEED * fixedDeltaTime.asSeconds()); }
		if (obj->moveLeft) { obj->moveX(-1*PLAYER_SPEED * fixedDeltaTime.asSeconds()); }
		if (obj->moveUp) { obj->moveY(-1 *PLAYER_SPEED * fixedDeltaTime.asSeconds()); }
		if (obj->moveDown) { obj->moveY(PLAYER_SPEED *fixedDeltaTime.asSeconds()); }
	}
};

