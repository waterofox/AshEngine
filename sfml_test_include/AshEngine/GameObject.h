//SFML includes:
//  
#include <SFML/Graphics.hpp>

#include <iostream>
#include <map>

namespace ash
{
	enum objectType
	{
		staticType = 0,
		dynamicType = 1
	};
	struct Sizef
	{
		float width = 0;
		float height = 0;
	};
	struct cords
	{
		int x;
		int y;
	};
	class GameObject
	{
		private:
		//obj
		sf::Sprite objSprite; 
		sf::Texture objTexture;
		std::string objTexturePath;
		sf::Vector2f objMovement;
		std::string name = "none";
		bool textureRepeated = false;

		//properties
		std::map<std::string, std::string>* customProperties = nullptr;
		
		bool visible = true;
		//animation
		bool isAnima = false;
		int frameCount = 1;
		float currentFrame = 0.f;
		int framePerSeconds = 24;
	public:
		GameObject();
		GameObject( const GameObject& objB);
		GameObject& operator=(const GameObject& objB)
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

			this->name = objB.name;

			if (this->customProperties != nullptr) { delete customProperties; customProperties = nullptr; }
			if (objB.customProperties != nullptr)
			{
				this->customProperties = new std::map<std::string, std::string>(*objB.customProperties);
			}

			return *this;
		}
		~GameObject();
		//get
		sf::Sprite& getSFMlobj(); //return sfml's core of object
		sf::Vector2f getPosition(); 
		Sizef getSize(); 
		std::string getName() { return this->name; }
		std::string getTexture() { return this->objTexturePath; }

		bool isVisible() { return this->visible; }
		bool isTextureRepeated() { return this->textureRepeated;}

		//set
		void setPosition(sf::Vector2f); //set object position
		void setX(float newX);
		void setY(float newY);
		void setScale(sf::Vector2f); 
		void setName(std::string name) { this->name = name; }
		void setTexturePath(std::string path) { this->objTexturePath = path; }
		void setVisible(bool arg) { this->visible = arg; }
		void setTextureRepeat(bool arg);

		void setTexture();
		void setCurrentFrame(int);
		void setFrameCount(int); 
		void setFramePerSeconds(int); //set fps of actual animation
		void disableAnimation();
		void enableAnimation(); 
		void updateTexture(std::string); //load texture by path
		
		void moveX(float plusX); //Ox move
		void moveY(float plusY); //Oy move
		void move(float plusX, float plusY);

		void setPropertiesSet(std::map<std::string, std::string>);
		std::string& operator[](std::string& key);
		std::string  operator[](std::string& key) const;
		std::string& operator[](const char* key);

		//chek
		bool isAnimated(); 
		//proc
		void updateAnimation(sf::Time deltaTime); //animation play method
	};
}