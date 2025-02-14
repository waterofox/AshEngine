#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <thread>


#include <SFML/Graphics.hpp>

namespace ash {
	class AshResourceManager
	{
	private:
		//dir
		std::string resourceDir;
		std::string assetsDir;
		std::string fontsDir;

		//textures & fonts data
		std::map<std::string, sf::Texture> loadedTextures;
		std::map<std::string, sf::Font>loadedFonts;

	public:

		AshResourceManager();
		~AshResourceManager() {}

		sf::Texture& loadTextureFromDir(const std::string& texturePath);
		void dropTexture(sf::Texture* texturePointer);

		sf::Font& loadFontFromDir(const std::string& fontPath);
		void dropFont(sf::Font* fontPointer);

		sf::Texture& getTexture(const std::string& textureName);
		sf::Font& getFont(const std::string& fontName);
	};
}