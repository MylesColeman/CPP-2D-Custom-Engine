#pragma once
#include "AnimationManager.h"
#include "CollisionRectangle.h"
#include <SFML/Graphics.hpp>

// Our base class for entities within our game world, inherits from sf::Sprite to allow easy drawing and manipulation of sprites
// Other classes can inherit from this to create more specific entity types - such as a player, ai, pickup etc
class Entity : public sf::Sprite
{
public:
    // Entity constructor for an animated entity
    explicit Entity(const Animation& animation) : 
        sf::Sprite(*animation.texture) 
    {
		this->setAnimation(animation);

		m_isAnimated = true;

		m_hitbox.m_height = m_animation->spriteHeight;
		m_hitbox.m_width = m_animation->spriteWidth;
    };

    // Entity constructor for a static, non-animated, entity
    explicit Entity(const StaticSprite& sprite) :
        sf::Sprite(*sprite.texture, sprite.textureRect)
    {
		m_animation = nullptr;
		m_isAnimated = false;

        m_hitbox.m_height = sprite.textureRect.size.y;
        m_hitbox.m_width = sprite.textureRect.size.x;
    };

	virtual ~Entity() = default; // Virtual destructor to ensure proper cleanup of derived classes

    // Sets the animation to be used, starting it on the first frame
    void setAnimation(const Animation& animation)
    {
        m_animation = &animation;
        this->setTexture(*m_animation->texture, true);
        m_currentFrame = 0;
        m_animClock.restart();
        m_isAnimated = true;

        // Calculates and sets the intRect for the first frame. Otherwise the entire spritesheet would be shown till it was set in update
        int intRectsYPos = m_currentFrame * m_animation->spriteHeight;
        this->setTextureRect(sf::IntRect({ 0, intRectsYPos }, { m_animation->spriteWidth, m_animation->spriteHeight }));
    }

	// Handles the updating of the entity, it is virtual so derived classes can extend it. But it handles animation by default
    virtual void update()
    {
		// After the set time, updates to the next sprite - if there is an animation set
        if (m_isAnimated && m_animation && m_animClock.getElapsedTime().asSeconds() > m_animation->timeBetweenFrames)
        {
            m_currentFrame++;

            // If reached the last frame, loops back around
            if (m_currentFrame >= m_animation->numFrames)
            {
                m_currentFrame = 0;
            }

            int intRectsYPos = m_currentFrame * m_animation->spriteHeight; // Sets the intRect's Y position based on the current frame 

            /* Updates the intRect, as all sprite sheets are composed vertically 'x' can remain as 0 but 'y' is updated based on the calculation above.The size
             is calculated using the spriteWidth and spriteHeight variables from the Animation struct */
            this->setTextureRect(sf::IntRect({ 0, intRectsYPos }, { m_animation->spriteWidth, m_animation->spriteHeight }));

            m_animClock.restart();
        }

		this->move(m_velocity); // Moves the entity based on its velocity

		// Updates the hitbox position ensuring it matches the entity's position - follows
		m_hitbox.m_xPos = this->getPosition().x;
		m_hitbox.m_yPos = this->getPosition().y;
    }

	const CollisionRectangle& getHitbox() const { return m_hitbox; } // Returns the hitbox of the entity for collision detection

protected:
    const Animation* m_animation{ nullptr };
    CollisionRectangle m_hitbox; // The hitbox for the entity, used for collision detection
    sf::Vector2f m_velocity{ 0.f, 0.f }; // The velocity of the entity, used for movement
private:
    sf::Clock m_animClock;
    int m_currentFrame{ 0 };

    bool m_isAnimated{ false };
};