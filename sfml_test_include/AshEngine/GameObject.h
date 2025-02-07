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
		dynamicType = 1,
		collisionType = 2
	};
	struct Sizef
	{
		float width = 0;
		float height = 0;

		Sizef(){}
		Sizef(float width, float height)
		{
			this->width = width;
			this->height = height;
		}
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
		sf::Vector2f objPreviosPosition;
		sf::Vector2f objMovement; 

		//collision
		bool collision = false;
		Sizef collisionSize;
		sf::Vector2f collisionCenter = sf::Vector2f(0,0);

		
		//animation //todo по хорошему нужно вырезать это и создать отдельный класс.
		bool isAnima = false;
		int frameCount = 1;
		int currentFrame = 0;
		int framePerSeconds = 24;
		sf::Time playTimer = sf::Time::Zero;

		//process methods
		void setTexture();

	public:
		//move 
		bool moveUp = false;
		bool moveRight = false;
		bool moveDown = false;
		bool moveLeft = false;

		//total set
		void clean();


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
			this->objPreviosPosition = objB.objPreviosPosition;
			this->collision = objB.collision;
			this->collisionSize = objB.collisionSize;
			this->collisionCenter = objB.collisionCenter;

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
		sf::Vector2f getPreviosPosition() { return objPreviosPosition; }
		bool isMoving();
		Sizef getSize(); 
		
		//getters <collision>
		bool isCollision() { return collision; }
		Sizef getCollisionSize() { return collisionSize; }
		sf::Vector2f getCollisionCenter() { return collisionCenter; }

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
		void setPreviosPosition(sf::Vector2f pos) { objPreviosPosition = pos; }
		void setX(float newX);
		void setY(float newY);
		void moveX(float plusX); //Ox move
		void moveY(float plusY); //Oy move
		void move(float plusX, float plusY);

		//setters <collision>
		void enableCollision() { collision = true; }
		void disableCollision() { collision = false; }
		void setCollisionSize(ash::Sizef size) { collisionSize = size; }
		void setCollisionSize(float width, float height) { collisionSize.width = width; collisionSize.height = height; }
		void setCollisionSize(sf::Vector2f size) { collisionSize.width = size.x; collisionSize.height = size.y; }
		void setCollisionCenter(sf::Vector2f center) { collisionCenter = center; }
		void setCollisionCenter(float x, float y) { collisionCenter.x = x; collisionCenter.y = y; }

		//settres <animation>
		void setCurrentFrame(int);
		void setFrameCount(int newFrameCount) { frameCount = newFrameCount; }
		void setFramePerSeconds(int newFramePerSeconds) { framePerSeconds = newFramePerSeconds; }
		void disableAnimation() { isAnima = false; }
		void enableAnimation() { isAnima = true; }
		void playAnimation(sf::Time deltaTime); 

	};
}