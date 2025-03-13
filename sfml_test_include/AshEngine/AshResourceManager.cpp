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
std::map<std::string, std::pair<sf::Texture*, AshResourceManager::textureSettings>>& ash::AshResourceManager::getMapOfLoadedTextures()
{
	return loadedTextures;
}

sf::Texture& AshResourceManager::getTexture(const std::string& textureName)
{
	auto textureIter = loadedTextures.find(textureName);
	if (textureIter == loadedTextures.end()) { return loadTextureFromDir(textureName);}
	std::pair<sf::Texture*, textureSettings>& foundTexture = (*textureIter).second;

	if (foundTexture.first == nullptr) { return loadTextureFromDir(textureName); }

	return (*foundTexture.first);
}

AshResourceManager::textureSettings& ash::AshResourceManager::getSettings(const std::string& textureName)
{
	return loadedTextures[textureName].second;
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


sf::Font& AshResourceManager::loadFontFromDir(const std::string& pathInDir)
{
	loadedFonts.insert(std::pair<std::string, sf::Font>(pathInDir, sf::Font()));
	loadedFonts[pathInDir].loadFromFile(resourceDir + fontsDir + pathInDir);

	return loadedFonts[pathInDir];
}