#include "GameLayout.h"

void ash::GameLayout::addObject(std::string uniqueName, GameObject obj,int type)
{
	switch (type)
	{
	case ash::objectType::dynamicType: {
		std::pair<std::string, GameObject> newObj(uniqueName, obj);
		dynamicObjects.insert(newObj);
	}break;
	case ash::objectType::staticType: {staticObjects.push_back(obj); }break;
	default:
		break;
	}
}

void ash::GameLayout::removeObject(std::string uniqueName)
{
	dynamicObjects.erase(uniqueName);
}

bool ash::GameLayout::getObject(std::string uniqueName, ash::GameObject*& buffer)
{
	if (dynamicObjects.find(uniqueName) != dynamicObjects.end())
	{
		buffer = &dynamicObjects[uniqueName];
		return true;
	}
	else { return false; }
}

void ash::GameLayout::sceneAnimationsUpdate(sf::Time deltaTime)
{
	for (auto obj = staticObjects.begin(); obj < staticObjects.end(); ++obj)
	{
		if (!obj->isVisible()) { continue; }
		if (obj->isAnimated()) { obj->updateAnimation(deltaTime); }
	}
	for (auto& element : dynamicObjects)
	{
		GameObject& obj = element.second;
		if (!obj.isVisible()) { continue; }
		if (obj.isAnimated()) { obj.updateAnimation(deltaTime); }
	}
}

void ash::GameLayout::renderScene(sf::RenderWindow& window)
{
	for (auto obj = staticObjects.begin(); obj < staticObjects.end(); ++obj)
	{
		if (!obj->isVisible()) { continue; }
		window.draw(obj->getSFMlobj());
	}
	for (auto& element : dynamicObjects)
	{
		GameObject& obj = element.second;
		if (!obj.isVisible()) { continue; }
		window.draw(obj.getSFMlobj());
	}

}
