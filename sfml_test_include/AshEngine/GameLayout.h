#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameObject.h"
#include <map>
#include <vector>
#include <string>

namespace ash
{
	class GameLayout 
	{
	private:
		std::map<std::string, GameObject> dynamicObjects;
		std::vector<GameObject> staticObjects;
	public:

		void addObject(std::string uniqueName, GameObject obj,int type); //add object to scene
		void removeObject(std::string uniqueName); //remove object from scene
		bool getObject(std::string uniqueName,GameObject*& buffer); //get object from scene by name
		void sceneAnimationsUpdate(sf::Time deltaTime);
		void renderScene(sf::RenderWindow& window);

		GameLayout() {}
		GameLayout(const GameLayout& SceneB)
		{
			dynamicObjects = SceneB.dynamicObjects;
			staticObjects = SceneB.staticObjects;
		}
		~GameLayout() {}

		GameLayout& operator=(const GameLayout& SceneB)
		{
			dynamicObjects = SceneB.dynamicObjects;
			staticObjects = SceneB.staticObjects;
			return *this;
		}
	};
}