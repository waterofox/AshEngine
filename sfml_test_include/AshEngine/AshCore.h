#pragma once
#include "AshEntity.h"
#include "AshResourceManager.h"
#include "AshAnimator.h"

#include "queue"
#include "fstream"

namespace ash {
	class AshCore
	{
		//some types
		using sceneType = std::map<int, std::map<std::string,AshEntity>>;
		using script = void(*)(AshCore*, AshEntity&);
		using eventHandling = void(*)(AshCore&);
		template <typename T>
		using mapByScene = std::map<std::string, T>;
	private:

		//resource
		AshResourceManager resourceManager;
		AshAnimator animator;

		//scene
		std::string sceneDir = "resources/scenes/";
		std::string actualSceneName;
		sceneType* actualScene = nullptr;
		bool sceneReady = false;
		
		//time
		sf::Time deltaTime = sf::Time::Zero;
		
		//window
		sf::RenderWindow mainWindow;
		unsigned int windowWidth;
		unsigned int windowHeight;
		std::string windowTitle;

		//process
		sf::Event actualEvent;
		eventHandling evHandlingFunction = nullptr;
		bool isCustomEvHandLoading = false;

		mapByScene<std::map<std::string, script>> scriptsBase;
		mapByScene<std::map<int, script>> slotsBase;

		std::queue<std::pair<int, AshEntity&>> signalsQueue;

		//game
		unsigned int framePerSeconds = 60;
		std::string properites = "resources/properties.txt";

		//camera
		sf::View camera;
		bool dynamicCamera = true;
		bool fullscreen = false;
		
		const sf::Vector2f& camSize = camera.getSize();
		const sf::Vector2f& camCenter = camera.getCenter();
		sf::Vector2f camPosition = sf::Vector2f(camCenter.x - camSize.x / 2, camCenter.y - camSize.y / 2);
		sf::FloatRect camBounds = sf::FloatRect(camPosition, camSize);

		//process methods
		void run();
		void eventHandlingStandart();
		void update();
		void render();
		void updateTextures();
		void updateCameraBounds();
		void processSignals();
		void updateEntity();
		void parsProperties(const std::string& propertyName, AshEntity& entity);
		void targetCollions();

	public:
		AshCore(const unsigned int& width, const int& height,const unsigned int& fps, const std::string& windowTitle);
		~AshCore() { if (actualScene != nullptr) { delete actualScene; actualScene = nullptr; } }

		//for usres
		void startEngine() { run(); }
		AshEntity& getEntity(const std::string& name);
		void emitSignal(const int& signlaID, AshEntity& entity);

		AshResourceManager& getResourceManager() { return resourceManager; }
		AshAnimator& getAnimator() { return animator; }

		//getters & setters <scene>
		sceneType* getActualScene() { return actualScene; }
		std::string getActualSceneName() { return actualSceneName; }

		void loadScene(const std::string& sceneName);
		void pushEntity(const AshEntity& entity, const int& lay);
		void pushEntity(const AshEntity& entity, const std::string& entityName, const int& lay);
		void addEntities(const std::map<std::string, AshEntity>& entitiesMap, const int& lay);
		void popEntity(const std::string& entitynName, const int& lay);
		void popEntity(const std::string& entityName);


		//Это потом. сейчас мега впадлу
		//todo человек может лично собирать сцену, испльхуя например метод сборки
		//человек будет создавать сцену ввиде указателя. Выделять под него помять и заполять. 
		//При смене сцены он будет отчищаться. Но у человека будет метод сборки конкретной сцены и её пустой указатель

		//todo нет. не пойдёт. напиши структуру сцене; 
		//void loadScene(const sceneType*& scene);

		//getters & setters <time>
		sf::Time getDeltaTime() { return deltaTime; }
		
		//getters & setters <window>
		sf::RenderWindow& getWindow() { return mainWindow; }

		//getters & setters <process>
		sf::Event& getActualEvent() { return actualEvent; }
		void addScript(const std::string& sceneName,const  std::string& entityName, script yourScript);
		void addSlot(const std::string& sceneName,const int& signalID, script yourSlot);
		void setEventHandlingFunction(eventHandling function) { evHandlingFunction = function; }
		


	};
}