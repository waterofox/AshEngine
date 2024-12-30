//SFML includes:
//  
#include <iostream>
#include <SFML/Graphics.hpp>


namespace Game
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

		bool visible = true;
		//animation
		bool isAnima = false;
		int frameCount = 1;
		float currentFrame = 0.f;
		int framePerSeconds = 24;
	public:

		//move
		bool moveUp = false;
		bool moveRight = false;
		bool moveDown = false;
		bool moveLeft = false;

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
			this->setTexture(this->objTexture);
			this->visible = objB.visible;

			this->moveUp = objB.moveUp;
			this->moveLeft = objB.moveLeft;
			this->moveRight = objB.moveRight;
			this->moveDown = objB.moveDown;

			this->name = objB.name;
			return *this;
		}
		~GameObject();
		//get
		sf::Sprite getSFMlobj(); //return sfml's core of object
		sf::Vector2f getPosition(); 
		Sizef getSize(); 
		std::string getName() { return this->name; }
		std::string getTexture() { return this->objTexturePath; }

		bool isVisible() { return this->visible; }
		//set
		void setPosition(sf::Vector2f); //set object position
		void setX(float newX);
		void setY(float newY);
		void setScale(sf::Vector2f); 
		void setName(std::string name) { this->name = name; }
		void setTexturePath(std::string path) { this->objTexturePath = path; }
		void setVisible(bool arg) { this->visible = arg; }

		void setTexture(sf::Texture&);
		void setCurrentFrame(int);
		void setFrameCount(int); 
		void setFramePerSeconds(int); //set fps of actual animation
		void disableAnimation();
		void enableAnimation(); 
		void updateTexture(std::string); //load texture by path
		
		void moveX(float plusX); //Ox move
		void moveY(float plusY); //Oy move
		void move(float plusX, float plusY);

		//chek
		bool isAnimated(); 
		//proc
		void updateAnimation(sf::Time deltaTime); //animation play method
	};
}