#pragma once
#include "AnimationManager.h"
#include "CollisionRectangle.h"
#include <SFML/Graphics.hpp>

enum class EntityType
{
    Standard,
    Player,
    Collectable,
    Bullet
};

// Our base class for entities within our game world, inherits from sf::Sprite to allow easy drawing and manipulation of sprites
// Other classes can inherit from this to create more specific entity types - such as a player, ai, pickup etc
class Entity : public sf::Sprite
{
public:
    // Entity constructor for an animated entity
    explicit Entity(const Animation& animation, EntityType type = EntityType::Standard) : 
        sf::Sprite(*animation.texture), m_type(type)
    {
		this->setAnimation(animation);

		m_isAnimated = true;

		m_hitbox.m_height = static_cast<float>(m_animation->spriteHeight);
		m_hitbox.m_width = static_cast<float>(m_animation->spriteWidth);

        this->setOrigin({ m_hitbox.m_width / 2.f, m_hitbox.m_height / 2.f });
    };

    // Entity constructor for a static, non-animated, entity
    explicit Entity(const StaticSprite& sprite, EntityType type = EntityType::Standard) :
        sf::Sprite(*sprite.texture, sprite.textureRect), m_type(type)
    {
		m_animation = nullptr;
		m_isAnimated = false;

        m_hitbox.m_height = static_cast<float>(sprite.textureRect.size.y);
        m_hitbox.m_width = static_cast<float>(sprite.textureRect.size.x);

        this->setOrigin({ m_hitbox.m_width / 2.f, m_hitbox.m_height / 2.f });
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
        if (!m_flipped)
        {
            // Face Right (Normal)
            this->setOrigin(m_animation->pivot);
            
            this->setTextureRect(sf::IntRect({ 0, intRectsYPos }, { m_animation->spriteWidth, m_animation->spriteHeight }));
        }
        else
        {
            // Face Left (Flipped)
            float flippedPivotX = m_animation->spriteWidth - m_animation->pivot.x;
            this->setOrigin({ flippedPivotX, m_animation->pivot.y });
            
            // Start X at width, and use negative width to flip
            this->setTextureRect(sf::IntRect({ m_animation->spriteWidth, intRectsYPos }, { -m_animation->spriteWidth, m_animation->spriteHeight }));
        }
    }

	// Handles the updating of the entity, it is virtual so derived classes can extend it. But it handles animation by default
    virtual void update(float deltaTime)
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

            m_animClock.restart();
        }

        if (m_animation)
        {
            /* Updates the intRect, as all sprite sheets are composed vertically 'x' can remain as 0 but 'y' is updated based on the calculation above.The size
                 is calculated using the spriteWidth and spriteHeight variables from the Animation struct */
            int intRectsYPos = m_currentFrame * m_animation->spriteHeight; // Sets the intRect's Y position based on the current frame 
            if (!m_flipped)
            {
                this->setOrigin(m_animation->pivot);
                this->setTextureRect(sf::IntRect({ 0, intRectsYPos }, { m_animation->spriteWidth, m_animation->spriteHeight }));
            }
            else
            {
                float flippedPivotX = m_animation->spriteWidth - m_animation->pivot.x;
                this->setOrigin({ flippedPivotX, m_animation->pivot.y });
                this->setTextureRect(sf::IntRect({ m_animation->spriteWidth, intRectsYPos }, { -m_animation->spriteWidth, m_animation->spriteHeight }));
            }
        }

		// Updates the hitbox position ensuring it matches the entity's position - follows
		this->syncHitbox();
    }

	// Syncs the hitbox to the entity's position, should be called after any position changes
    void syncHitbox()
    {
        m_hitbox.m_xPos = this->getPosition().x - (m_hitbox.m_width / 2.f);
        m_hitbox.m_yPos = this->getPosition().y - (m_hitbox.m_height / 2.f);
    }

    void destroy() { m_destroy = true; } // Sets the entity to be destroyed

    bool getDestroy() const { return m_destroy; }
    EntityType getType() const { return m_type; } 
	const CollisionRectangle& getHitbox() const { return m_hitbox; } // Returns the hitbox of the entity for collision detection

protected:
    const Animation* m_animation{ nullptr };
    EntityType m_type;
    CollisionRectangle m_hitbox; // The hitbox for the entity, used for collision detection
    bool m_destroy{ false };

    bool m_flipped{ false };
    void flipSprite(bool flipped) { m_flipped = flipped; }
private:
    sf::Clock m_animClock;
    int m_currentFrame{ 0 };

    bool m_isAnimated{ false };
};