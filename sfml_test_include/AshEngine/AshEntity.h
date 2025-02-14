#pragma once
#include <iostream>
#include <string>
#include <map>

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
		bool drawable = true;//todo �����������!!!!!
		bool updatable = false; //todo ����� �� ������������ ������ �������, ���� �� �� � ���� ��������� 
		bool visible = true;

		//properties
		std::map<std::string, int>* p_intProperties = nullptr;
		std::map<std::string, float>* p_floatProperties = nullptr;
		std::map<std::string, bool>* p_boolProperties = nullptr;

		//move
		sf::Vector2f previousPosition = sf::Vector2f(0,0);

		//collision
		bool colliding = false;
		sf::FloatRect collisionSize = sf::FloatRect(0,0,0,0);
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
		std::string getName() { return entityName; }

		void setName(std::string name) { entityName = name; }

		//getters & setters <draw>
		bool isDrawable() { return drawable; }
		bool isVisible() { return visible; }
		bool isUpdatable() { return updatable; }

		void setDrawable(bool arg) { drawable = arg; }
		void setVisible(bool arg) { visible = arg; }
		void setUpdatable(bool arg) { updatable = arg; }

		//getters & setters <properties>
		template <typename T>
		void addProperty(int p_type, std::string key, T value)
		{
			switch (p_type)
			{
			case p_int:
			{
				if (p_intProperties != nullptr)
				{
					p_intProperties->insert(key, value);
				}
				else
				{
					p_intProperties = new std::map<std::string, int>();
					p_intProperties->insert(key, value);
				}
			}
			case p_float:
			{
				if (p_floatProperties != nullptr)
				{
					p_floatProperties->insert(key, value);
				}
				else
				{
					p_floatProperties = new std::map<std::string, float>();
					p_floatProperties->insert(key, value);
				}
			}
			case p_bool:
			{
				if (p_boolProperties != nullptr)
				{
					p_boolProperties->insert(key, value);
				}
				else
				{
					p_boolProperties = new std::map<std::string, bool>();
					p_boolProperties->insert(key, value);
				}
			}
			default:
				break;
			}
		}

		template <typename T>
		T& operator[](const std::string& key)
		{
			if (p_intProperties != nullptr)
			{
				auto& propertyIterator = p_intProperties->find(key);
				if (propertyIterator != p_intProperties->end())
				{
					return (*p_intProperties)[key];
				}
			}
			if (p_floatProperties != nullptr)
			{
				auto& propertyIterator = p_floatProperties->find(key);
				if (propertyIterator != p_floatProperties->end())
				{
					return (*p_floatProperties)[key];
				}
			}
			if (p_boolProperties != nullptr)
			{
				auto& propertyIterator = p_boolProperties->find(key);
				if (propertyIterator != p_boolProperties->end())
				{
					return (*p_boolProperties)[key];
				}
			}
			return nullptr;
		}
		template <typename T>
		T& operator[](const char*& key)
		{
			return (*this)[std::string(key)];
		}

		//getters & setters <move>
		sf::Vector2f getPreviousPosition() { return previousPosition; }

		void setPreviousPosition(sf::Vector2f pos) { previousPosition = pos; }
		void stopMoving();

		//getters & setters <collision>
		bool isColliding() { return colliding; }
		sf::FloatRect getCollisionSize() { return collisionSize; }
		sf::Vector2f getCollisionPosition() { return collisionPosition; }

		void setColliding(bool arg) { colliding = arg; }
		void setCollisionSize(sf::FloatRect size) { collisionSize = size; }
		void setCollisionPosition(sf::Vector2f pos) { collisionPosition = pos; }
 	};
};
