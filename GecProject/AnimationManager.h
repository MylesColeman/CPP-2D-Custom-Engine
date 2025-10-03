#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

// What an animation needs
struct Animation
{
	sf::Texture* texture{ nullptr };
	int numFrames{ 0 };
	int spriteWidth{ 0 };
	int spriteHeight{ 0 };
	float timeBetweenFrames{ 0.1f }; // How often the sprite is updated - animated
};

class AnimationManager
{
public:
	AnimationManager(TextureManager& texureManager) : m_textureManager(texureManager)
	{
		// Zombie Animation Setup
		// Loading the sprite sheets and configuring the animations
		configureAnimation("zombieIdle", "Data/Textures/MaleZombie/idle_combined.png", 15);
		configureAnimation("zombieWalk", "Data/Textures/MaleZombie/walk_combined.png", 10);
		configureAnimation("zombieAttack", "Data/Textures/MaleZombie/attack_combined.png", 8);
		configureAnimation("zombieDeath", "Data/Textures/MaleZombie/dead_combined.png", 12);
	}

	// Assigns variables to the structure, so the animation can be used
	void configureAnimation(const std::string animName, const std::string textureFilePath, int frameCount, float animSpeed = 0.1f)
	{
		sf::Texture* texture = m_textureManager.getTexture(textureFilePath);
		// Checks if there isn't a texture
		if (texture == nullptr)
		{
			std::cout << "Could not configure animation " << animName << "! Due to a missing texture!" << std::endl;
			return;
		}

		Animation anim;

		anim.texture = texture;
		anim.numFrames = frameCount;
		anim.spriteWidth = texture->getSize().x;
		anim.spriteHeight = texture->getSize().y / frameCount;
		anim.timeBetweenFrames = animSpeed;

		m_animations[animName] = anim;
	}

	// Used to access the animation in other scripts
	const Animation& getAnimation(const std::string& animName) const
	{
		return m_animations.at(animName);
	}
private:
	TextureManager& m_textureManager;

	std::map<std::string, Animation> m_animations;
};