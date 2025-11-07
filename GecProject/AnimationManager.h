#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

// What an animation needs
struct Animation
{
	sf::Texture* texture{ nullptr };
	int numFrames{ 0 };
	int spriteWidth{ 0 };
	int spriteHeight{ 0 };
	float timeBetweenFrames{ 0.1f }; // How often the sprite is updated - animated
};

// What a static sprite needs
struct StaticSprite
{
	sf::Texture* texture{ nullptr };
	sf::IntRect textureRect;
};

class AnimationManager
{
public:
	AnimationManager(TextureManager& texureManager) : m_textureManager(texureManager)
	{
		// Loading the sprite sheets
		// Animations
		// Zombie Animation Setup
		configureAnimation("zombieIdle", "Data/Textures/MaleZombie/idle_combined.png", 15);
		configureAnimation("zombieWalk", "Data/Textures/MaleZombie/walk_combined.png", 10);
		configureAnimation("zombieAttack", "Data/Textures/MaleZombie/attack_combined.png", 8);
		configureAnimation("zombieDeath", "Data/Textures/MaleZombie/dead_combined.png", 12);

		// Statics
		// World
		configureStaticSprite("WalledFloor", "Data/Textures/World/tilemap_packed.png", sf::IntRect({ 0,0 }, { 18,18 }));
	}

	// Assigns variables to the structure, so the animation can be used
	void configureAnimation(const std::string& animName, const std::string& textureFilePath, int frameCount, float animSpeed = 0.1f)
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

	void configureStaticSprite(const std::string& spriteName, const std::string& textureFilePath, const sf::IntRect& textureRect)
	{
		sf::Texture* texture = m_textureManager.getTexture(textureFilePath);
		// Checks if there isn't a texture
		if (texture == nullptr)
		{
			std::cout << "Could not configure static sprite " << spriteName << "! Due to a missing texture!" << std::endl;
			return;
		}

		StaticSprite sprite;

		sprite.texture = texture;
		sprite.textureRect = textureRect;

		m_staticSprites[spriteName] = sprite;
	}

	// Used to access the animation in other scripts
	const Animation& getAnimation(const std::string& animName) const
	{
		return m_animations.at(animName);
	}
	// Used to access the sprite in other scripts
	const StaticSprite& getStaticSprite(const std::string& spriteName) const
	{
		return m_staticSprites.at(spriteName);
	}
private:
	TextureManager& m_textureManager;

	std::map<std::string, Animation> m_animations;
	std::map<std::string, StaticSprite> m_staticSprites;
};