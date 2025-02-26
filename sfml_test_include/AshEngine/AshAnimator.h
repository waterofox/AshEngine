#pragma once
#include <SFML/System.hpp>
#include "AshEntity.h"

#include <iostream>
#include <string>
#include <map>
namespace ash
{
	class AshAnimator
	{
	public:
		struct animation
		{
			int frameCount = 1;
			int framePerSeconds = 24;
			int currentFrame = 0;
			bool enable = false;
			sf::Time animationTimer = sf::Time::Zero;
			animation() {}
			animation(const animation& animationB)
			{
				frameCount = animationB.frameCount;
				framePerSeconds = animationB.framePerSeconds;
				currentFrame = animationB.currentFrame;
				enable = animationB.enable;
				animationTimer = animationB.animationTimer;
			}
			void clear()
			{
				frameCount = 1;
				framePerSeconds = 24;
				currentFrame = 0;
				enable = false;
				animationTimer = sf::Time::Zero;
			}
		};

	private:
		std::map<std::string, animation> animationsLibrary;
	public:
		AshAnimator() {}
		~AshAnimator() {}

		void updateAnimation(const sf::Time& deltaTime, AshEntity& entity);
		void addAnimation(const std::string& texture, const int& frameCount, const int& fps, bool enabel);
		void addAnimation(const std::string& texture, const animation& anima);
		void removeAnimation(const std::string& textureToRemove);

		void setEnabelAnimation(const std::string& textureToEnabelAnimation, const bool& arg);
		void setAnimationFps(const std::string& textureToEnabelAnimation, const int& fps);
		void setCurrentFrame(const std::string& textureToEnabelAnimation, const int& frame);

		animation& getAnimation(const std::string& texture);
	};
}