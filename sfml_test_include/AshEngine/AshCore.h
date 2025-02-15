#pragma once
#include "AshEntity.h"
#include "AshResourceManager.h"
#include "queue"

namespace ash {
	class AshCore
	{
		//some types
		using sceneType = std::map<int, std::vector<AshEntity>>;
		using script = void(*)(AshCore*, AshEntity*);
		using eventHandling = void(*)(AshCore&);
		template <typename T>
		using mapByScene = std::map<std::string, T>;
	private:

		//resource
		AshResourceManager resourceManager;

		//scene
		std::string sceneDir = "../scenes/";
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

		mapByScene<std::pair<std::string, script>> scriptsBase;
		mapByScene<std::map<int, script>> slotsBase;

		std::queue<std::pair<int, AshEntity*>> signalsQueue;

		//game
		unsigned int framePerSeconds = 60;
		std::string properites = "../properties.txt";

		//camera
		sf::View camera;
		bool dynamicCamera = false;
		bool fullscreen = false;

		//process methods
		void run();
		void eventHandlingStandart();
		void update();
		void render();
		void targetCollions();

	public:
		AshCore(const unsigned int& width, const int& height,const unsigned int& fps, const std::string& windowTitle);
		~AshCore() { if (actualScene != nullptr) { delete actualScene; actualScene = nullptr; } }

		void startEngine() { run(); }

		AshResourceManager& getResourceManager() { return resourceManager; }

		//getters & setters <scene>
		sceneType* getActualScene() { return actualScene; }
		std::string getActualSceneName() { return actualSceneName; }

		void loadScene(const std::string& sceneName);

		//getters & setters <time>
		sf::Time getDeltaTime() { return deltaTime; }

		//getters & setters <window>
		sf::RenderWindow& getWindow() { return mainWindow; }

		//getters & setters <process>
		sf::Event& getActualEvent() { return actualEvent; }
		void setEventHandlingFunction(eventHandling function) { evHandlingFunction = function; }

	};
}