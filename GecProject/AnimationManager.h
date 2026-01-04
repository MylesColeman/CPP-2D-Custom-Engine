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
	sf::Vector2f pivot{ 0.f, 0.f }; // Where the animation should pivot around, for flipping
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
		// Player
		configureAnimation("playerIdle", "Data/Textures/Player/skeleton_idle.png", 4, 0.7f);
		configureAnimation("playerJump", "Data/Textures/Player/skeleton_jump.png", 7, 0.15f);
		configureAnimation("playerJumpShot", "Data/Textures/Player/skeleton_jumpShot.png", 8, 0.0625f);
		configureAnimation("playerStandingShot", "Data/Textures/Player/skeleton_standingShot.png", 5, 0.1f);
		configureAnimation("playerWalk", "Data/Textures/Player/skeleton_walk.png", 6);
		configureAnimation("playerWalkShot", "Data/Textures/Player/skeleton_walkShot.png", 8, 0.0625f);

		// Setting the pivot point of sprites that flip
		float spineX = 6.5f;
		float spineY = m_animations["playerIdle"].spriteHeight / 2.f;
		setAnimationPivot("playerIdle", { spineX, spineY });
		spineY = m_animations["playerJump"].spriteHeight / 2.f;
		setAnimationPivot("playerJump", { spineX, spineY });
		spineY = m_animations["playerJumpShot"].spriteHeight / 2.f;
		setAnimationPivot("playerJumpShot", { spineX, spineY });
		spineY = m_animations["playerWalk"].spriteHeight / 2.f;
		setAnimationPivot("playerWalk", { spineX, spineY });
		spineY = m_animations["playerWalkShot"].spriteHeight / 2.f;
		setAnimationPivot("playerWalkShot", { spineX, spineY });
		spineX = 7.5f;
		spineY = m_animations["playerStandingShot"].spriteHeight / 2.f;
		setAnimationPivot("playerStandingShot", { spineX, spineY });

		// Statics
		// Bullet
		configureStaticSprite("bullet", "Data/Textures/bullet.png", sf::IntRect({ 0, 0 }, { 5, 5 }));

		// World
		int columns = 16;
		int rows = 7;
		int tileWidth = 18;
		int tileHeight = 18;

		// Loop from 0 to 111 (112 tile)
		for (int i = 0; i < (columns * rows); ++i)
		{
			// Calculate X and Y coordinates in the texture
			int x = (i % columns) * tileWidth; // Modulus for column position
			int y = (i / columns) * tileHeight; // Division for row position

			// Generate a unique name
			std::string tileName = "tile_" + std::to_string(i);

			// Create the sprite configuration automatically
			configureStaticSprite(tileName, "Data/Textures/World/tilemap_packed.png", sf::IntRect({ x, y }, { tileWidth, tileHeight }));
		}

		// Door
		configureStaticSprite("door", "Data/Textures/World/door.png", sf::IntRect({ 0, 0 }, { 21, 28 }));
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
		anim.pivot = { anim.spriteWidth / 2.f, anim.spriteHeight / 2.f }; // Sets the default pivot to be the centre of the sprite

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
	const Animation& getAnimation(const std::string& animName) const { return m_animations.at(animName); }

	// Used to access the sprite in other scripts
	const StaticSprite& getStaticSprite(const std::string& spriteName) const { return m_staticSprites.at(spriteName); }

	// Used to manually alter the pivor point of an animation
	void setAnimationPivot(const std::string& animName, sf::Vector2f newPivot)
	{
		if (m_animations.count(animName)) { m_animations[animName].pivot = newPivot; }
	}
private:
	TextureManager& m_textureManager;

	std::unordered_map<std::string, Animation> m_animations;
	std::unordered_map<std::string, StaticSprite> m_staticSprites;
};