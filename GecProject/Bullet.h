#pragma once
#include "DynamicEntity.h"

class Bullet : public DynamicEntity
{
public:
    Bullet(const StaticSprite& sprite)
        : DynamicEntity(sprite, EntityType::Bullet)
    {
        m_gravity = 0.f; // Disables gravity for the the bullet
    }

    // Sets the bullet up when its been fired - activated
    void fire(sf::Vector2f position, sf::Vector2f velocity)
    {
        m_active = true;
        this->setPosition(position);
        m_velocity = velocity;
        m_lifetime = 3.f;    // Reset the timer
        this->syncHitbox();  // Ensure hitbox is at the new position immediately
    }

	// Used to deactivate the bullet when its lifetime is over or it hits something
    void deactivate()
    {
        m_active = false;
    }

	bool isActive() const { return m_active; } // Checks if the bullet is currently active

	// Deactivates the bullet after its lifetime is over
    void update(float deltaTime) override
    {
        m_lifetime -= deltaTime;
        if (m_lifetime <= 0.f)
        {
			deactivate();
        }

        DynamicEntity::update(deltaTime); // Calls the base class update to handle animation and movement
    }
private:
    float m_lifetime{ 3.f }; // How long the bullet will last before being deactivated
    bool m_active{ false };
};