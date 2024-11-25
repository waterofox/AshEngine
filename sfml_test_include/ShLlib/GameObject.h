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
	class GameObject
	{
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
			return *this;
		}
		~GameObject();
		//get
		sf::Sprite getSFMlobj(); //возвращает SFML объект для отрисовки в окне
		sf::Vector2f getPosition(); //Возвращает текущую позицию (левый верхний угол)
		Sizef getSize(); //возвращается текущего sf::Sprite с учётом скейла
		//set
		void setPosition(sf::Vector2f); //изменение позиции объекта
		void setScale(sf::Vector2f); //изменение скейла
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
	private:
		//obj
		sf::Sprite objSprite; 
		sf::Texture objTexture;
		sf::Vector2f objMovement;
		//animation
		bool isAnima = false;
		int frameCount = 1;
		float currentFrame = 0.f;
		int framePerSeconds = 24;
	};
}