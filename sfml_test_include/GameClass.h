#pragma once
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
	sf::Time fixedDeltaTime = sf::seconds(1.6 / 60.f);

	sf::Event actualEvent;
	sf::Keyboard::Key key;
	sf::RenderWindow window;
	sf::View camera;
	//game fields
	Game::GameScene* scene = nullptr;
	
public:
//work staff
	void run();
	void eventHandling();
	void update();
	void render();
	void playerInput(sf::Keyboard::Key,bool isPressed);
	bool loadScene(std::string path);
//important staff
	//constructors
	GameClass(){
		scene = new Game::GameScene;
		loadScene("resources/scenes/testScene.txt");
		camera.setSize(sf::Vector2f(640,480));
	}
	//destructors
	~GameClass() { delete scene; }

	//scripts
	void controlScript(Game::GameObject& obj)
	{
		if (obj.moveRight) { obj.moveX(100 * fixedDeltaTime.asSeconds()); }
		if (obj.moveLeft) { obj.moveX(-100 * fixedDeltaTime.asSeconds()); }
		if (obj.moveUp) { obj.moveY(-100 * fixedDeltaTime.asSeconds()); }
		if (obj.moveDown) { obj.moveY(100 * fixedDeltaTime.asSeconds()); }
	}
};

