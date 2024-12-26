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
		sf::Sprite getSFMlobj(); //возвращает SFML объект для отрисовки в окне
		sf::Vector2f getPosition(); //Возвращает текущую позицию (левый верхний угол)
		Sizef getSize(); //возвращается текущего sf::Sprite с учётом скейла
		std::string getName() { return this->name; }
		std::string getTexture() { return this->objTexturePath; }

		bool isVisible() { return this->visible; }
		//set
		void setPosition(sf::Vector2f); //изменение позиции объекта
		void setX(float newX);
		void setY(float newY);
		void setScale(sf::Vector2f); //изменение скейла
		void setName(std::string name) { this->name = name; }
		void setTexturePath(std::string path) { this->objTexturePath = path; }
		void setVisible(bool arg) { this->visible = arg; }

		void setTexture(sf::Texture&); // переприсовение текстуры (оно тебе не нужно, это сугубо для удобства работы основного методв подгрузки текстуры)
		void setCurrentFrame(int);//изменяет текущий кадр анимации
		void setFrameCount(int); //изменяет кол-во кадров в анимации
		void setFramePerSeconds(int); //изменяет кол-во кадров в секунду
		void disableAnimation(); //отключает анимацию
		void enableAnimation(); //включает анимацию
		void updateTexture(std::string); //основной метод подгрузки текстуры
		
		void moveX(float plusX); //смещение объекта по Ox согласно заданному парметру
		void moveY(float plusY); //смещение объекта по Oy согласно заданному парметру
		void move(float plusX, float plusY);

		//chek
		bool isAnimated(); //Проверка на то, включена ли анимация 
		//proc
		void updateAnimation(sf::Time deltaTime); //метод который обновляет текущий кадр анимаци (считай проигрывает её)
	};
}