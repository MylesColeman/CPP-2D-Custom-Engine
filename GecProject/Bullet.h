#pragma once
#include "DynamicEntity.h"

class Bullet : public DynamicEntity
{
public:
    Bullet(const StaticSprite& sprite, sf::Vector2f velocity)
        : DynamicEntity(sprite, EntityType::Bullet)
    {
        m_velocity = velocity; // Velocity only needs setting once
        m_gravity = 0.f; // Disables gravity for the the bullet
    }

    void update(float deltaTime) override
    {
        m_lifetime -= deltaTime;
        if (m_lifetime <= 0.f)
        {
            this->destroy();
        }

        DynamicEntity::update(deltaTime);
    }
private:
    float m_lifetime{ 3.f }; // How long the bullet will last before being destroyed
};