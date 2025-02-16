#include "AshResourceManager.h"

using namespace ash;

AshResourceManager::AshResourceManager()
{
	resourceDir = "resources/";
	assetsDir = "gameassets/";
	fontsDir = "fonts/";
}
sf::Texture& AshResourceManager::loadTextureFromDir(const std::string& pathInDir)
{
	loadedTextures.insert(std::pair<std::string,sf::Texture>(pathInDir,sf::Texture()));
	loadedTextures[pathInDir].loadFromFile(resourceDir + assetsDir + pathInDir);
	
	return loadedTextures[pathInDir];
}
void  AshResourceManager::dropTexture(const sf::Texture* texturePointer)
{
	for (auto& texture : loadedTextures)
	{
		//todo верятно я даун и эта хйня работате медленно
		if (&(texture.second) == texturePointer)
		{
			loadedTextures.erase(texture.first);
			texturePointer = nullptr;
			return;
		}
	}
}
sf::Font& AshResourceManager::loadFontFromDir(const std::string& pathInDir)
{
	loadedFonts.insert(std::pair<std::string, sf::Font>(pathInDir, sf::Font()));
	loadedFonts[pathInDir].loadFromFile(resourceDir + fontsDir + pathInDir);

	return loadedFonts[pathInDir];
}
void AshResourceManager::dropFont(const sf::Font* fontPointer)
{
	for (auto& font : loadedFonts)
	{
		if (&(font.second) == fontPointer)
		{
			loadedFonts.erase(font.first);
			fontPointer = nullptr;
			return;
		}
	}
}

sf::Texture& AshResourceManager::getTexture(const std::string& textureName)
{
	auto textureIter = loadedTextures.find(textureName);
	if (textureIter != loadedTextures.end())
	{
		return (*textureIter).second;
	}
	return loadTextureFromDir(textureName);
}

sf::Font& AshResourceManager::getFont(const std::string& fontName)
{
	auto fontIter = loadedFonts.find(fontName);
	if (fontIter != loadedFonts.end())
	{
		return (*fontIter).second;
	}
	return loadFontFromDir(fontName);
}
