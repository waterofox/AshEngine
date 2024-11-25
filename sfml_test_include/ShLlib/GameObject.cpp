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
	this->setTexture(this->objTexture);
}
GameObject::~GameObject() {}
sf::Sprite GameObject::getSFMlobj() { return objSprite; }
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
void GameObject::setCurrentFrame(int newCurrentFrame) { currentFrame = newCurrentFrame; }
void GameObject::setTexture(sf::Texture& newTexture)
{
	setFrameCount(newTexture.getSize().x/newTexture.getSize().y);
	setCurrentFrame(0.f);
	objSprite.setTexture(newTexture);
	objSprite.setTextureRect(sf::IntRect(0,0,newTexture.getSize().y,newTexture.getSize().y));
	if (frameCount > 1) { enableAnimation(); }
}
void GameObject::setFramePerSeconds(int newFramePerSeconds) { framePerSeconds = newFramePerSeconds; }
void GameObject::disableAnimation() { isAnima = false; }
void GameObject::enableAnimation() { isAnima = true; }
bool GameObject::isAnimated() { return isAnima; }
void GameObject::updateAnimation(sf::Time deltaTime)
{
	if (!isAnima) { return; }
	currentFrame += (int(framePerSeconds/2) + 1)*deltaTime.asSeconds();
	objSprite.setTextureRect(sf::IntRect(int(currentFrame)*(objTexture.getSize().x/frameCount), 0, objTexture.getSize().y, objTexture.getSize().y));
	if (int(currentFrame) >= frameCount-1) { currentFrame = 0.f; }
}
void GameObject::updateTexture(std::string path)
{
	objTexture.loadFromFile(path);
	setTexture(objTexture);
}
void GameObject::moveX(float plusX) { objMovement.x += plusX; objSprite.setPosition(objMovement); }
void GameObject::moveY(float plusY) { objMovement.y += plusY; objSprite.setPosition(objMovement); }
void GameObject::move(float plusX, float plusY) { objMovement.x += plusX; objMovement.y += plusY; objSprite.setPosition(objMovement);}