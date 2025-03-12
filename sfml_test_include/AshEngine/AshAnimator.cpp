#include "AshAnimator.h"
using namespace ash;
void ash::AshAnimator::updateAnimation(const sf::Time& deltaTime, AshEntity& entity)
{
	auto animationIter = animationsLibrary.find(entity.getTexturePath());
	if (animationIter != animationsLibrary.end())
	{
		animation& animationToPlay = (*animationIter).second;
		animationToPlay.animationTimer += deltaTime;
		if (animationToPlay.animationTimer.asSeconds() >= 1.0 / animationToPlay.framePerSeconds)
		{
			animationToPlay.animationTimer = sf::Time::Zero;
			animationToPlay.currentFrame = (animationToPlay.currentFrame + 1) % animationToPlay.frameCount;
		}
		entity.setTextureRect(sf::IntRect(int(animationToPlay.currentFrame) * (entity.getTexture()->getSize().x / animationToPlay.frameCount), 0, entity.getTexture()->getSize().y, entity.getTexture()->getSize().x / animationToPlay.frameCount));
	}
}

void ash::AshAnimator::addAnimation(const std::string& texture, const int& frameCount, const int& fps, bool enabel)
{
	animation anima;
	anima.frameCount = frameCount;
	anima.framePerSeconds = fps;
	anima.enable = enabel;
	addAnimation(texture, anima);
}

void ash::AshAnimator::addAnimation(const std::string& texture, const animation& anima)
{
	std::pair<std::string, animation> buffer;
	buffer.first = texture;
	buffer.second =  anima;
	animationsLibrary.insert(buffer);
}

void ash::AshAnimator::removeAnimation(const std::string& textureToRemove)
{
	auto animationIter = animationsLibrary.find(textureToRemove);
	if (animationIter != animationsLibrary.end())
	{
		animationsLibrary.erase(animationIter);
	}
}

void ash::AshAnimator::setEnabelAnimation(const std::string& textureToEnabelAnimation, const bool& arg)
{
	auto animationIter = animationsLibrary.find(textureToEnabelAnimation);
	if (animationIter != animationsLibrary.end())
	{
		(*animationIter).second.enable = arg;
		return;
	}
	std::cout << "ANIMATION ERROR: no animation for <" + textureToEnabelAnimation + ">\n";
	
}

void ash::AshAnimator::setAnimationFps(const std::string& textureToEnabelAnimation, const int& fps)
{
	auto animationIter = animationsLibrary.find(textureToEnabelAnimation);
	if (animationIter != animationsLibrary.end())
	{
		(*animationIter).second.framePerSeconds = fps;
		return;
	}
	std::cout << "ANIMATION ERROR: no animation for <" + textureToEnabelAnimation + ">\n";
}

void ash::AshAnimator::setCurrentFrame(const std::string& textureToEnabelAnimation, const int& frame)
{
	auto animationIter = animationsLibrary.find(textureToEnabelAnimation);
	if (animationIter != animationsLibrary.end())
	{
		(*animationIter).second.currentFrame = frame;
		return;
	}
	std::cout << "ANIMATION ERROR: no animation for <" + textureToEnabelAnimation + ">\n";
}

ash::AshAnimator::animation& ash::AshAnimator::getAnimation(const std::string& texture)
{
	auto animationIter = animationsLibrary.find(texture);
	if (animationIter != animationsLibrary.end())
	{
		return (*animationIter).second;
	}
	std::cout << "ANIMATION ERROR: no animation for <" + texture + ">\n";
}
