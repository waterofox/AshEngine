#include "AshEngine/AshCore.h"
using namespace ash;
void eventHandlingFunction(AshCore& theCore)
{
	sf::RenderWindow& window = theCore.getWindow();
	sf::Event& actualEvent = theCore.getActualEvent();
	if(window.pollEvent(actualEvent))
	{
		switch (actualEvent.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed: {std::cout << actualEvent.key.code; }
		default:
			break;
		}
	}
}
int main()
{
	AshCore engine(640, 480, 60, "AshEngineWindow");
	engine.setEventHandlingFunction(eventHandlingFunction);
	engine.startEngine();
}