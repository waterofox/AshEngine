#include "AshEngine/AshCore.h"
using namespace ash;

#define playerSpeed 100

void catchKeyBoardEvent(AshCore& core,sf::Keyboard::Key key, bool arg)
{
	AshEntity& player = core.getEntity("player");
	switch (key)
	{
	case sf::Keyboard::W: {player.moveUp = arg; }break;
	case sf::Keyboard::A: {player.moveLeft = arg; }break;
	case sf::Keyboard::S: {player.moveDown = arg; }break;
	case sf::Keyboard::D:{player.moveRight = arg;}break;
	default:
		break;
	}
}
void eventHandlingFunction(AshCore& theCore)
{
	sf::RenderWindow& window = theCore.getWindow();
	sf::Event& actualEvent = theCore.getActualEvent();
	if(window.pollEvent(actualEvent))
	{
		AshEntity& player = theCore.getEntity("player");
		switch (actualEvent.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		case sf::Event::KeyPressed: {catchKeyBoardEvent(theCore, actualEvent.key.code, true); } break;
		case sf::Event::KeyReleased: {catchKeyBoardEvent(theCore, actualEvent.key.code, false); } break;
		default:
			break;
		}
	}
}

enum signals
{
	elka_signal_in = 1,
	elka_signal_out = 2,
};

void testSlotIN(AshCore* core, AshEntity& senderEntity)
{
	AshResourceManager& manager = core->getResourceManager();
	manager.getTexture(senderEntity.getTexturePath()).setSmooth(true);
	senderEntity.getBool("pressed") = true;
}
void testSlotOUT(AshCore* core, AshEntity& senderEntity)
{
	AshResourceManager& manager = core->getResourceManager();
	manager.getTexture(senderEntity.getTexturePath()).setSmooth(false);
	senderEntity.getBool("pressed") = false;
}


void scriptForTetsPropertysAndSignals(AshCore* core, AshEntity& entity)
{
	AshEntity& player = core->getEntity("player");
	if (entity.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		if (!entity.getBool("pressed")) { core->emitSignal(elka_signal_in,entity); }
	}
	else
	{
		if (entity.getBool("pressed")) { core->emitSignal(elka_signal_out, entity); }
	}
}
/*
void scriptForTetsTextureSettings(AshCore* core, AshEntity& entity)
{
	AshEntity& player = core->getEntity("player");
	if (entity.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		sf::Texture& textureOFEntity = core->getResourceManager().getTexture(entity.getTexturePath());
		//todo Такой способ не является безопастным, тк. это скрипт цикла. 
		// Если вы попытаетесь манипулировать с текстурой объекта вне поля видимости, вы можете получить ошибку об ограничении прав доступа, т.к текстуры не будет существовать в памяти.
		// НЕобходимо выполнять проверку entity.isDrawable() если да - окей, лазай ручками
		// Если нет, то используйте метод   getSettings и манипулируйте полученным объектом. Это безопастно и при появлении объекта в поле видимости. Он приметнит ваши новые настройки.
		textureOFEntity.setSmooth(true);
		return;
	}
	if (!entity.isDrawable())
	{
		AshResourceManager::textureSettings& settingsToManipulate = core->getResourceManager().getSettings(entity.getTexturePath());
		if (settingsToManipulate.smooth) { settingsToManipulate.smooth = false; }
	}
}
*/
void testScript(AshCore* core, AshEntity& entity)
{
	if (entity.moveUp)
	{
		entity.move(sf::Vector2f(0, -playerSpeed) * core->getDeltaTime().asSeconds());
	}
	if (entity.moveRight)
	{
		entity.move(sf::Vector2f(playerSpeed, 0) * core->getDeltaTime().asSeconds());
	}
	if (entity.moveDown)
	{
		entity.move(sf::Vector2f(0, playerSpeed) * core->getDeltaTime().asSeconds());
	}
	if (entity.moveLeft)
	{
		entity.move(sf::Vector2f(-playerSpeed,0) * core->getDeltaTime().asSeconds());
	}
}

int main()
{
	AshCore engine(640, 480, 120, "AshEngineWindow");
	engine.setEventHandlingFunction(eventHandlingFunction);

	engine.addSlot("scene",elka_signal_in, testSlotIN);
	engine.addSlot("scene", elka_signal_out, testSlotOUT);

	engine.addScript("scene", "player", testScript);
	engine.addScript("scene", "elka2", scriptForTetsPropertysAndSignals);
	engine.loadScene("testScene.txt");
	
	engine.startEngine();
}