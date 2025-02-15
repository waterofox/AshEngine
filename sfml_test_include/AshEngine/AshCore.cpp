#include "AshCore.h"

void ash::AshCore::run()
{
	mainWindow.create(sf::VideoMode(windowWidth, windowHeight), windowTitle);
	mainWindow.setFramerateLimit(framePerSeconds);
	mainWindow.setKeyRepeatEnabled(false);

	sf::Clock clock;

	//game loop
	while (mainWindow.isOpen())
	{
		deltaTime = clock.restart();
		if(evHandlingFunction != nullptr)
		{
			evHandlingFunction(*this);
		}
		else { eventHandlingStandart(); }
		update();
		render();

	}
}

void ash::AshCore::eventHandlingStandart()
{
	if (mainWindow.pollEvent(actualEvent))
	{
		switch (actualEvent.type)
		{
		case sf::Event::Closed: {mainWindow.close(); } break;
		default:
			break;
		}
	}
}

void ash::AshCore::update()
{
}

void ash::AshCore::render()
{
	mainWindow.clear();
	mainWindow.display();
}

ash::AshCore::AshCore(const unsigned int& width, const int& height, const unsigned int& fps, const std::string& windowTitle)
{
	framePerSeconds = fps;
	windowWidth = width;
	windowHeight = height;
	this->windowTitle = windowTitle;
}