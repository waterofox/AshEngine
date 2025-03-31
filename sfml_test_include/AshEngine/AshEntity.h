#pragma once
#include <iostream>
#include <string>
#include <map>
#include "variant"

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>

namespace ash
{
	enum PropertiesTypes
	{
		p_int = 1,
		p_float = 2,
		p_bool = 3,
	};

	class AshEntity : public sf::Sprite
	{
		//private fields
	private:
		std::string entityName = "none";

		//draw
		std::string texturePath;
		bool drawable = false;
		bool updatable = false; //todo нужно ли просчитывать логику объекта, пока он не в хоне видимости 
		bool visible = true;

		//properties
		std::map<std::string, int>* p_intProperties = nullptr;
		std::map<std::string, float>* p_floatProperties = nullptr;
		std::map<std::string, bool>* p_boolProperties = nullptr;

		//move
		sf::Vector2f previousPosition = sf::Vector2f(0,0);

		//collision
		bool colliding = false;
		sf::Vector2f collisionSize = sf::Vector2f(0,0);
		sf::Vector2f collisionPosition = sf::Vector2f(0,0);

		//public fields
	public:
		
		//move fields
		bool moveUp = false;
		bool moveLeft = false;
		bool moveDown = false;
		bool moveRight = false;

	private:
		void clearProperties();

	public:
		//constructors & distructor & operator = 
		AshEntity() {}
		AshEntity(const AshEntity& entityB);

		~AshEntity();

		AshEntity& operator=(const AshEntity& entityB);

		//set
		void setToDefault();

		//getters & setters <name>
		std::string getName() const { return entityName; }

		void setName(std::string name) { entityName = name; }

		//getters & setters <draw>
		std::string getTexturePath() const { return texturePath; }
		bool isDrawable() const { return drawable; }
		bool isVisible() const { return visible; }
		bool isUpdatable() const { return updatable; }
		
		void setTexturePath(std::string path) { texturePath = path; }
		void setDrawable(bool arg) { drawable = arg; }
		void setVisible(bool arg) { visible = arg; }
		void setUpdatable(bool arg) { updatable = arg; }

		//getters & setters <properties>
		template <typename T>
		void addProperty(PropertiesTypes p_type, std::string key, T value)
		{
			switch (p_type)
			{
			case p_int:
			{
				std::pair<std::string, int> newProperty(key, value);
				if (p_intProperties != nullptr)
				{
					p_intProperties->insert(newProperty);
				}
				else
				{
					p_intProperties = new std::map<std::string, int>();
					p_intProperties->insert(newProperty);
				}
			} break;
			case p_float:
			{
				std::pair<std::string, float> newProperty(key, value);
				if (p_floatProperties != nullptr)
				{
					p_floatProperties->insert(newProperty);
				}
				else
				{
					p_floatProperties = new std::map<std::string, float>();
					p_floatProperties->insert(newProperty);
				}
			}break;
			case p_bool:
			{
				std::pair<std::string, bool> newProperty(key, value);
				if (p_boolProperties != nullptr)
				{
					p_boolProperties->insert(newProperty);
				}
				else
				{
					p_boolProperties = new std::map<std::string, bool>();
					p_boolProperties->insert(newProperty);
				}
			}break;
			default:
				break;
			}
		}
		int& getInt(const std::string& key) const;
		float& getFloat(const std::string& key) const;
		bool& getBool(const std::string& key) const;

		//getters & setters <move>
		sf::Vector2f getPreviousPosition() const { return previousPosition; }

		void setPreviousPosition(sf::Vector2f pos) { previousPosition = pos; }
		void stopMoving();

		//getters & setters <collision>
		bool isColliding() const { return colliding; }
		sf::Vector2f getCollisionSize() const { return collisionSize; }
		sf::Vector2f getCollisionPosition() const { return collisionPosition; }

		void setColliding(bool arg) { colliding = arg; }
		void setCollisionSize(sf::Vector2f size) { collisionSize = size; }
		void setCollisionPosition(sf::Vector2f pos) { collisionPosition = pos; }
 	};
};
