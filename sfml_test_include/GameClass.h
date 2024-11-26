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
class GameClass
{
private:
	//processing fields
	sf::Clock clock;
	sf::Time timeSinceLastUodate = sf::Time::Zero;
	sf::Time fixedDeltaTime = sf::seconds(1.6 / FPS);

	sf::Event actualEvent;
	sf::Keyboard::Key key;
	sf::RenderWindow window;
	sf::View camera;
	//game fields
	std::vector<Game::GameScene>* scene = nullptr;
	
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
		loadScene("resources/scenes/testScene.txt");
		camera.setSize(sf::Vector2f(640,480));
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

