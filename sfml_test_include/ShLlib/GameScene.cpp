#include "GameScene.h"

void Game::GameScene::addObject(std::string uniqueName, GameObject obj,int type)
{
	switch (type)
	{
	case Game::objectType::dynamicType: {
		std::pair<std::string, GameObject> newObj(uniqueName, obj);
		dynamicObjects.insert(newObj);
	}break;
	case Game::objectType::staticType: {staticObjects.push_back(obj); }break;
	default:
		break;
	}
}

void Game::GameScene::removeObject(std::string uniqueName)
{
	dynamicObjects.erase(uniqueName);
}

bool Game::GameScene::getObject(std::string uniqueName, Game::GameObject*& buffer)
{
	if (dynamicObjects.find(uniqueName) != dynamicObjects.end())
	{
		buffer = &dynamicObjects[uniqueName];
		return true;
	}
	else { return false; }
}

void Game::GameScene::sceneAnimationsUpdate(sf::Time deltaTime)
{
	for (auto obj = staticObjects.begin(); obj < staticObjects.end(); ++obj)
	{
		if (obj->isAnimated()) { obj->updateAnimation(deltaTime); }
	}
	for (auto& element : dynamicObjects)
	{
		GameObject& obj = element.second;
		if (obj.isAnimated()) { obj.updateAnimation(deltaTime); }
	}
}

void Game::GameScene::renderScene(sf::RenderWindow& window)
{
	for (auto obj = staticObjects.begin(); obj < staticObjects.end(); ++obj)
	{
		window.draw(obj->getSFMlobj());
	}
	for (auto& element : dynamicObjects)
	{
		GameObject& obj = element.second;
		window.draw(obj.getSFMlobj());
	}

}
