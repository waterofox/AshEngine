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
		//the core
		sf::Sprite objSprite; 
		std::string name = "none";

		//texturing
		sf::Texture objTexture;
		std::string objTexturePath;
		bool textureRepeated = false;
		bool visible = true;


		//properties
		std::map<std::string, std::string>* customProperties = nullptr;
		sf::Vector2f objMovement; 
		
		//animation //todo по хорошему нужно вырезать это и создать отдельный класс.
		bool isAnima = false;
		int frameCount = 1;
		int currentFrame = 0;
		int framePerSeconds = 24;
		sf::Time playTimer = sf::Time::Zero;

		//process methods
		void setTexture();

	public:
		//constructors
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

		//geters <the core>
		sf::Sprite& getSFMlobj() { return objSprite; }; //return sfml's core of object
		std::string getName() { return this->name; }

		//getters <texturing>
		sf::Texture& getTexture() { return this->objTexture; }
		std::string getTexturePath() { return this->objTexturePath; }
		bool isVisible() { return this->visible; }
		bool isTextureRepeated() { return this->textureRepeated; }

		//getters <properties>
		std::string& operator[](std::string& key);
		std::string  operator[](std::string& key) const;
		std::string& operator[](const char* key);
		std::string  operator[](const char* key)  const;
		sf::Vector2f getPosition(); 
		Sizef getSize(); 

		//getters <animation>
		bool isAnimated() { return isAnima; }
		int getFrameCount() { return this->frameCount; }
		int getCurrentFrame() { return this->currentFrame; }
		int getFramePresSeconds() { return this->framePerSeconds; }

		//setters <the core>
		void setName(std::string name) { this->name = name; }

		//settres <texturing>
		void setTexturePath(std::string path) { this->objTexturePath = path; }
		void setVisible(bool arg) { this->visible = arg; }
		void setTextureRepeat(bool arg);
		void loadTexture(std::string); 

		//setters <properties>
		void setPropertiesSet(std::map<std::string, std::string>);
		void setScale(sf::Vector2f newScale) { objSprite.setScale(newScale); }
		void setPosition(sf::Vector2f); //set object position
		void setX(float newX);
		void setY(float newY);
		void moveX(float plusX); //Ox move
		void moveY(float plusY); //Oy move
		void move(float plusX, float plusY);

		//settres <animation>
		void setCurrentFrame(int);
		void setFrameCount(int newFrameCount) { frameCount = newFrameCount; }
		void setFramePerSeconds(int newFramePerSeconds) { framePerSeconds = newFramePerSeconds; }
		void disableAnimation() { isAnima = false; }
		void enableAnimation() { isAnima = true; }
		void playAnimation(sf::Time deltaTime); 

	};
}