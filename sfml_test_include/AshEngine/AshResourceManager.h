#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <thread>


#include <SFML/Graphics.hpp>


namespace ash {
	class AshResourceManager
	{
	public:

		struct textureSettings
		{
			bool repeated = false;
			bool smooth = false;
			bool sRgb = false;

			unsigned int countOfVisibleEntity = 0;
			
			textureSettings() {}
			textureSettings(const textureSettings& settigns)
			{
				repeated = settigns.repeated;
				smooth = settigns.smooth;
				sRgb = settigns.sRgb;
				countOfVisibleEntity = settigns.countOfVisibleEntity;
			}
			void clear()
			{
				repeated = false;
				smooth = false;
				sRgb = false;
			}
			void updateTextureSettings(const sf::Texture& textureInVideoMemory)
			{
				repeated = textureInVideoMemory.isRepeated();
				smooth = textureInVideoMemory.isSmooth();
				sRgb = textureInVideoMemory.isSrgb();
			}
		};
	private:
		//dir
		std::string resourceDir;
		std::string assetsDir;
		std::string fontsDir;

		//textures & fonts data
		std::map<std::string, std::pair<sf::Texture*, textureSettings>> loadedTextures;


		//todo переделай методы шрифтов на подобии методов текстур
		std::map<std::string, sf::Font>loadedFonts;

	public:

		AshResourceManager();
		~AshResourceManager() {}

		sf::Texture& loadTextureFromDir(const std::string& texturePath);
		void dropTexture(const std::string& pathInDir);
		void addTexture(const std::string, const AshResourceManager::textureSettings settings);
		std::map<std::string, std::pair<sf::Texture*, textureSettings>>& getMapOfLoadedTextures();


		sf::Font& loadFontFromDir(const std::string& fontPath);
		void dropFont(const sf::Font* fontPointer);

		sf::Texture& getTexture(const std::string& textureName);
		textureSettings& getSettings(const std::string& textureName);
		sf::Font& getFont(const std::string& fontName);
	};
}