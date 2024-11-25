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
		sf::Sprite getSFMlobj(); //���������� SFML ������ ��� ��������� � ����
		sf::Vector2f getPosition(); //���������� ������� ������� (����� ������� ����)
		Sizef getSize(); //������������ �������� sf::Sprite � ������ ������
		//set
		void setPosition(sf::Vector2f); //��������� ������� �������
		void setScale(sf::Vector2f); //��������� ������
		void setTexture(sf::Texture&); // �������������� �������� (��� ���� �� �����, ��� ������ ��� �������� ������ ��������� ������ ��������� ��������)
		void setCurrentFrame(int);//�������� ������� ���� ��������
		void setFrameCount(int); //�������� ���-�� ������ � ��������
		void setFramePerSeconds(int); //�������� ���-�� ������ � �������
		void disableAnimation(); //��������� ��������
		void enableAnimation(); //�������� ��������
		void updateTexture(std::string); //�������� ����� ��������� ��������
		void moveX(float plusX); //�������� ������� �� Ox �������� ��������� ��������
		void moveY(float plusY); //�������� ������� �� Oy �������� ��������� ��������
		void move(float plusX, float plusY);
		//chek
		bool isAnimated(); //�������� �� ��, �������� �� �������� 
		//proc
		void updateAnimation(sf::Time deltaTime); //����� ������� ��������� ������� ���� ������� (������ ����������� �)
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