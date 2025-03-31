#include "AshEntity.h"
using namespace ash;

#define entity (*this)

void ash::AshEntity::clearProperties()
{
	if (entity.p_intProperties != nullptr)
	{
		delete entity.p_intProperties;
		entity.p_intProperties = nullptr;
	}
	if (entity.p_floatProperties != nullptr)
	{
		delete entity.p_floatProperties;
		entity.p_floatProperties = nullptr;
	}
	if (entity.p_boolProperties != nullptr)
	{
		delete entity.p_boolProperties;
		entity.p_boolProperties = nullptr;
	}
}

AshEntity::AshEntity(const AshEntity& entityB)
{
	entity.setScale(entityB.getScale());
	entity.setPosition(entityB.getPosition());
	entity.setTextureRect(entityB.getTextureRect());
	entity.setOrigin(entityB.getOrigin());

	entity.entityName = entityB.entityName;
	entity.texturePath = entityB.texturePath;

	entity.drawable = entityB.drawable;
	entity.updatable = entityB.updatable;
	entity.visible = entityB.visible;

	entity.clearProperties();

	if (entityB.p_intProperties != nullptr)
	{
		entity.p_intProperties = new std::map<std::string, int>((*entityB.p_intProperties));
	}
	if (entityB.p_floatProperties != nullptr)
	{
		entity.p_floatProperties = new std::map<std::string, float>((*entityB.p_floatProperties));
	}
	if (entityB.p_boolProperties != nullptr)
	{
		entity.p_boolProperties = new std::map<std::string, bool>((*entityB.p_boolProperties));
	}

	entity.previousPosition = entityB.previousPosition;

	entity.colliding = entityB.colliding;
	entity.collisionSize = entityB.collisionSize;
	entity.collisionPosition = entityB.collisionPosition;

	entity.moveUp = entityB.moveUp;
	entity.moveLeft = entityB.moveLeft;
	entity.moveDown = entityB.moveDown;
	entity.moveRight = entityB.moveRight;
	
}
AshEntity& AshEntity::operator=(const AshEntity& entityB)
{
	entity.setScale(entityB.getScale());
	entity.setPosition(entityB.getPosition());
	entity.setTextureRect(entityB.getTextureRect());
	entity.setOrigin(entityB.getOrigin());

	entity.entityName = entityB.entityName;
	entity.texturePath = entityB.texturePath;

	entity.drawable = entityB.drawable;
	entity.updatable = entityB.updatable;
	entity.visible = entityB.visible;

	entity.clearProperties();

	if (entityB.p_intProperties != nullptr)
	{
		entity.p_intProperties = new std::map<std::string, int>((*entityB.p_intProperties));
	}
	if (entityB.p_floatProperties != nullptr)
	{
		entity.p_floatProperties = new std::map<std::string, float>((*entityB.p_floatProperties));
	}
	if (entityB.p_boolProperties != nullptr)
	{
		entity.p_boolProperties = new std::map<std::string, bool>((*entityB.p_boolProperties));
	}

	entity.previousPosition = entityB.previousPosition;

	entity.colliding = entityB.colliding;
	entity.collisionSize = entityB.collisionSize;
	entity.collisionPosition = entityB.collisionPosition;

	entity.moveUp = entityB.moveUp;
	entity.moveLeft = entityB.moveLeft;
	entity.moveDown = entityB.moveDown;
	entity.moveRight = entityB.moveRight;

	return entity;
}

AshEntity::~AshEntity()
{
	clearProperties();
}

void AshEntity::setToDefault()
{
	entity.entityName = "none";

	entity.setTexture(sf::Texture());

	clearProperties();

	entity.previousPosition = sf::Vector2f(0, 0);

	entity.colliding = false;
	entity.collisionSize = sf::Vector2f(0, 0);
	entity.collisionPosition = sf::Vector2f(0, 0);
	
	stopMoving();
}

int& ash::AshEntity::getInt(const std::string& key) const
{
	auto propertyIter = p_intProperties->find(key);
	if (propertyIter != p_intProperties->end())
	{
		return (*propertyIter).second;
	}
	throw std::out_of_range("ENTITY ERROR: " + entityName + " property <" + key + "> not found");
}

float& ash::AshEntity::getFloat(const std::string& key) const
{
	auto propertyIter = p_floatProperties->find(key);
	if (propertyIter != p_floatProperties->end())
	{
		return (*propertyIter).second;
	}
	throw std::out_of_range("ENTITY ERROR: " + entityName + " property <" + key + "> not found");
}

bool& ash::AshEntity::getBool(const std::string& key) const
{
	auto propertyIter = p_boolProperties->find(key);
	if (propertyIter != p_boolProperties->end())
	{
		return (*propertyIter).second;
	}
	throw std::out_of_range("ENTITY ERROR: " + entityName + " property <" + key + "> not found");
}

void ash::AshEntity::stopMoving()
{
	moveUp = false;
	moveLeft = false;
	moveDown = false;
	moveRight = false;
}
