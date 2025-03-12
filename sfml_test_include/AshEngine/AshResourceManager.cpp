#include "AshResourceManager.h"

using namespace ash;

AshResourceManager::AshResourceManager()
{
	resourceDir = "resources/";
	assetsDir = "gameassets/";
	fontsDir = "fonts/";
}
sf::Texture& AshResourceManager::loadTextureFromDir(const std::string& pathInDir)
{/*
	loadedTextures.insert(std::pair<std::string,sf::Texture>(pathInDir,sf::Texture()));
	loadedTextures[pathInDir].loadFromFile(resourceDir + assetsDir + pathInDir);
	
	return loadedTextures[pathInDir];
	*/

	//todo вот этот кощмар перепиши пожалуйста как будет свободное время
	auto textureIter = loadedTextures.find(pathInDir);
	if (textureIter != loadedTextures.end())
	{
		textureSettings& settings = (*textureIter).second.second;
		sf::Texture*& textureLPointer = (*textureIter).second.first;
		
		if (textureLPointer != nullptr) { delete textureLPointer; textureLPointer = nullptr; }
		textureLPointer = new sf::Texture();
		sf::Texture& ourTexture = (*textureLPointer);
		ourTexture.loadFromFile(resourceDir + assetsDir + pathInDir);

		ourTexture.setRepeated(settings.repeated);
		ourTexture.setSmooth(settings.smooth);
		ourTexture.setSrgb(settings.sRgb);
	}
	else
	{
		std::pair<sf::Texture*, textureSettings> newTexture(nullptr,textureSettings());
		loadedTextures.insert(std::pair<std::string,std::pair<sf::Texture*,textureSettings>>(pathInDir, newTexture));

		sf::Texture*& textureLPointer = loadedTextures[pathInDir].first;
		textureSettings& settings = loadedTextures[pathInDir].second;

		textureLPointer = new sf::Texture();
		sf::Texture& ourTexture = (*textureLPointer);
		ourTexture.loadFromFile(resourceDir + assetsDir + pathInDir);

		ourTexture.setRepeated(settings.repeated);
		ourTexture.setSmooth(settings.smooth);
		ourTexture.setSrgb(settings.sRgb);
	}
	return (*loadedTextures[pathInDir].first);
}
/*
void  AshResourceManager::dropTexture(const sf::Texture* texturePointer)
{
	for (auto& texture : loadedTextures)
	{
		
		if (&(texture.second) == texturePointer)
		{
			loadedTextures.erase(texture.first);
			texturePointer = nullptr;
			return;
		}
	}
}
*/
void AshResourceManager::dropTexture(const std::string& pathIndir)
{
	auto textureIter = loadedTextures.find(pathIndir);
	if (textureIter == loadedTextures.end()) { return; }

	sf::Texture*& texturePointer = (*textureIter).second.first;
	if (texturePointer != nullptr) { delete texturePointer; texturePointer = nullptr; }
}
void ash::AshResourceManager::addTexture(const std::string pathInDir, const AshResourceManager::textureSettings settings)
{
	sf::Texture* newTexturePtr = nullptr;
	std::pair<sf::Texture*, textureSettings> newData(newTexturePtr, settings);
	loadedTextures.insert(std::pair<std::string,std::pair<sf::Texture*,textureSettings>>(pathInDir, newData));
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
	//todo вот эту хуйню ножно переписать (у тебя блять всегда существует контейнир под текстуру. Там просто может быть nullptr на Texture)
	auto textureIter = loadedTextures.find(textureName);
	if (textureIter != loadedTextures.end())
	{
		//return (*textureIter).second;
		return (*(*textureIter).second.first);
	}
	return loadTextureFromDir(textureName);
}

AshResourceManager::textureSettings& ash::AshResourceManager::getSettings(const std::string& textureName)
{
	//todo а тут блять можно протсо вернуть настройки
	auto textureIter = loadedTextures.find(textureName);
	if (textureIter != loadedTextures.end())
	{
		//return (*textureIter).second;
		return (*(*textureIter).second.first);
	}
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
