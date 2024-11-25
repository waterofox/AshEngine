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

		//void loadSceneFile(std::string filePath);
		void addObject(std::string uniqueName, GameObject obj,int type); //�������� ������ �� �����
		void removeObject(std::string uniqueName); // ������� ������ �� �����
		GameObject& getObject(std::string uniqueName); //�������� ������ ����� �� ������
		void sceneAnimationsUpdate(sf::Time deltaTime);
		void renderScene(sf::RenderWindow& window); //������ ������ ����� �����

		GameScene() {}
		//GameScene(std::string filePath);
		~GameScene() {}
	};
}