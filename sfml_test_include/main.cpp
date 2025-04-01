#include "AshEngine/AshCore.h"
using namespace ash;

struct block : public AshEntity
{
	sf::Vector2f realPositionOnLay;
};
void initBlock(block& newBlock)
{
	newBlock.setScale(sf::Vector2f(1, 1));
	newBlock.setTextureRect(sf::IntRect(0, 0, 64, 64));
	newBlock.setVisible(true);
	newBlock.setColliding(false);
}
void chunkLoader(AshCore* core, const::std::string chankId)
{
	std::ifstream chunkData("resources/chunks/" + chankId + ".txt");
	std::string buffer;
	sf::Vector2f positionOfFirst;
	sf::Vector2f positionOfPrivious;
	int layNum = 0;
	while (!chunkData.eof())
	{
		++layNum;
		chunkData >> buffer;
		int bloksCount = std::stoi(buffer);
		for (int i = 0; i < bloksCount; ++ i)
		{
			block newBlock;
			chunkData >> buffer;
			newBlock.setTexturePath("statickAssets/terrain/" + buffer + ".png");
			std::string name;
			chunkData >> buffer;
			name += buffer + ' ';
			newBlock.realPositionOnLay.x = std::stof(buffer);
			chunkData >> buffer;
			name += buffer + ' ';
			newBlock.realPositionOnLay.y = std::stof(buffer);
			chunkData >> buffer;
			name += buffer;
			
			newBlock.setName(name);
			initBlock(newBlock);


			if (i == 0 and layNum == 1)
			{
				positionOfFirst = sf::Vector2f(newBlock.realPositionOnLay.x + 32, newBlock.realPositionOnLay.y + 16);
				newBlock.setPosition(positionOfFirst);
				positionOfPrivious = positionOfFirst;
				core->pushEntity(newBlock, layNum - 1);
				continue;
			}
			else if (i == 0)
			{
				positionOfFirst = sf::Vector2f(positionOfFirst.x - 32, positionOfFirst.y + 16);
				newBlock.setPosition(positionOfFirst);
				positionOfPrivious = positionOfFirst;
				core->pushEntity(newBlock, layNum - 1);
				continue;
			}
			
			newBlock.setPosition(positionOfPrivious.x + 32, positionOfPrivious.y + 16);
			core->pushEntity(newBlock, layNum - 1);
			positionOfPrivious = sf::Vector2f(positionOfPrivious.x + 32, positionOfPrivious.y + 16);

			
		}
		//ты смог распарсить чанк. А что елси по тому же приципу распологать чанки. Тно уже складывать их на отдельные слои. Получается, что я буду собирать мини островки из блоков. + это облегчает задчу поворота.
		// потому что повернуть чанки по отдельности - легче, чем поворачивать вообще всё
	}
}

void eventHandlingFunction(AshCore& theCore)
{
	sf::RenderWindow& window = theCore.getWindow();
	sf::Event& actualEvent = theCore.getActualEvent();
	if(window.pollEvent(actualEvent))
	{

		switch (actualEvent.type)
		{
		case sf::Event::Closed: {window.close(); } break;
		default:
			break;
		}
	}
}

int main()
{
	AshCore engine(640, 640, 120, "AshEngineWindow");
	engine.setEventHandlingFunction(eventHandlingFunction);

	engine.loadScene("blocks.txt");

	chunkLoader(&engine, "1");

	engine.startEngine();
}