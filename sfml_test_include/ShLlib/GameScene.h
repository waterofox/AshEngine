#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameObject.h"
#include <map>
#include <vector>
#include <string>

namespace Game
{
	class GameScene 
	{
	private:
		std::map<std::string, GameObject> dynamicObjects;
		std::vector<GameObject> staticObjects;
	public:

		void addObject(std::string uniqueName, GameObject obj,int type); //добавить объект на сцену
		void removeObject(std::string uniqueName); // удалить объект со сцены
		bool getObject(std::string uniqueName,Game::GameObject*& buffer); //получить объект сцены по ссылке
		void sceneAnimationsUpdate(sf::Time deltaTime);
		void renderScene(sf::RenderWindow& window); //рендер нового кадра сцены

		GameScene() {}
		GameScene(const Game::GameScene& SceneB)
		{
			dynamicObjects = SceneB.dynamicObjects;
			staticObjects = SceneB.staticObjects;
		}
		~GameScene() {}

		Game::GameScene& operator=(const Game::GameScene& SceneB)
		{
			dynamicObjects = SceneB.dynamicObjects;
			staticObjects = SceneB.staticObjects;
			return *this;
		}
	};
}