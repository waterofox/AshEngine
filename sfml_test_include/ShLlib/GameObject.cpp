#include "GameObject.h"
using namespace Game;
GameObject::GameObject() {}
Game::GameObject::GameObject(const GameObject& objB)
{
	this->currentFrame = objB.currentFrame;
	this->frameCount = objB.frameCount;
	this->framePerSeconds = objB.framePerSeconds;
	this->isAnima = objB.isAnima;
	this->objMovement = objB.objMovement;
	this->objSprite = objB.objSprite;
	this->objTexture = objB.objTexture;
	this->setTexture();
	this->visible = objB.visible;

	this->textureRepeated = objB.textureRepeated;
	this->setTextureRepeat(this->textureRepeated);
	this->objSprite.setTextureRect(objB.objSprite.getTextureRect());

	this->moveUp = objB.moveUp;
	this->moveLeft = objB.moveLeft;
	this->moveRight = objB.moveRight;
	this->moveDown = objB.moveDown;

	this->name = objB.name;

	if (this->customProperties != nullptr) { delete customProperties; customProperties = nullptr; }
	if (objB.customProperties != nullptr)
	{
		this->customProperties = new std::map<std::string, std::string>(*objB.customProperties);
	}
}
GameObject::~GameObject() {}
sf::Sprite& GameObject::getSFMlobj() { return objSprite; }
sf::Vector2f GameObject::getPosition() { return objSprite.getGlobalBounds().getPosition(); }
Sizef GameObject::getSize()
{
	Sizef objectSizef;
	objectSizef.width = objSprite.getGlobalBounds().getSize().x;
	objectSizef.height = objSprite.getGlobalBounds().getSize().y;
	return objectSizef;
}
void GameObject::setPosition(sf::Vector2f newPosition) 
{ 
	objSprite.setPosition(newPosition);
	objMovement = newPosition;
}
void Game::GameObject::setX(float newX)
{
	sf::Vector2f newPos = this->getPosition();
	newPos.x = newX;
	this->setPosition(newPos);
}
void Game::GameObject::setY(float newY)
{
	sf::Vector2f newPos = this->getPosition();
	newPos.y = newY;
	this->setPosition(newPos);
}
void GameObject::setScale(sf::Vector2f newScale) { objSprite.setScale(newScale); }
void GameObject::setFrameCount(int newFrameCount) { frameCount = newFrameCount; }
void GameObject::setCurrentFrame(int newCurrentFrame) 
{
	currentFrame = newCurrentFrame;
	objSprite.setTextureRect(sf::IntRect(int(currentFrame) * (objTexture.getSize().x / frameCount), 0, objTexture.getSize().y, objTexture.getSize().y));
}
void Game::GameObject::setTextureRepeat(bool arg)
{
	objTexture.setRepeated(arg);
	textureRepeated = arg;
	objSprite.setTexture(objTexture);
}
void GameObject::setTexture()
{
	setFrameCount(objTexture.getSize().x/objTexture.getSize().y);
	setCurrentFrame(0.f);
	objSprite.setTexture(objTexture);
	objSprite.setTextureRect(sf::IntRect(0,0,objTexture.getSize().y,objTexture.getSize().y));
	if (frameCount > 1 and isAnima) { enableAnimation(); }
}
void GameObject::setFramePerSeconds(int newFramePerSeconds) { framePerSeconds = newFramePerSeconds; }
void GameObject::disableAnimation() { isAnima = false; }
void GameObject::enableAnimation() { isAnima = true; }
bool GameObject::isAnimated() { return isAnima; }
void GameObject::updateAnimation(sf::Time deltaTime)
{
	if (!isAnima) { return; }
	currentFrame += (int(framePerSeconds/frameCount) + 1)*deltaTime.asSeconds();
	if (currentFrame >= frameCount) { currentFrame -= frameCount; }
	objSprite.setTextureRect(sf::IntRect(int(currentFrame)*(objTexture.getSize().x/frameCount), 0, objTexture.getSize().y, objTexture.getSize().y));
}
void GameObject::updateTexture(std::string path)
{
	objTexture.loadFromFile(path);
	objTexturePath = path;
	setTexture();
}
void GameObject::moveX(float plusX) { objMovement.x += plusX; objSprite.setPosition(objMovement); }
void GameObject::moveY(float plusY) { objMovement.y += plusY; objSprite.setPosition(objMovement); }
void GameObject::move(float plusX, float plusY) { objMovement.x += plusX; objMovement.y += plusY; objSprite.setPosition(objMovement);}

void Game::GameObject::setPropertiesSet(std::map<std::string, std::string> newPattern)
{
	if (customProperties != nullptr) { delete customProperties; }
	customProperties = new std::map<std::string, std::string>(newPattern);
}

std::string& Game::GameObject::operator[](std::string& key)
{
	if (customProperties == nullptr) 
	{ 
		std::cout << "OBJECT ERROR<" << name << ">: No custom properties" << std::endl;
		return key;
	}
	std::map<std::string, std::string>&  stats = *(customProperties);
	if (stats.find(key) == stats.end()) 
	{ 
		std::cout << "OBJECT ERROR<" << name << ">: No property <" << key << "> in custom properties" << std::endl;
		return key; 
	}
	return stats[key];
}

std::string Game::GameObject::operator[](std::string& key) const
{
	if (customProperties == nullptr) 
	{ 
		std::cout << "OBJECT ERROR<" << name << ">: No custom properties" << std::endl; 
		return key; 
	}
	std::map<std::string, std::string>& stats = *(customProperties);
	if (stats.find(key) == stats.end()) 
	{ 
		std::cout << "OBJECT ERROR<" << name << ">: No property <" << key << "> in custom properties" << std::endl;
		return key; 
	}
	return stats[key];
}

std::string& Game::GameObject::operator[](const char* key)
{
	if (customProperties == nullptr)
	{
		std::cout << "OBJECT ERROR<" << name << ">: No custom properties" << std::endl;
		std::string stringKey(key);
		return stringKey;
	}
	std::map<std::string, std::string>& stats = *(customProperties);
	if (stats.find(key) == stats.end())
	{
		std::cout << "OBJECT ERROR<" << name << ">: No property <" << key << "> in custom properties" << std::endl;
		std::string stringKey(key);
		return stringKey;
	}
	return stats[key];
}
