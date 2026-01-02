#pragma once
#include "Entity.h"

// Entity that is affected by physics, gravity and/or velocity
class DynamicEntity : public Entity
{
public:
	using Entity::Entity; // Inherit constructors

	virtual void update(float deltaTime) override
	{
		m_velocity.y += m_gravity * deltaTime; // Applies gravity to the player's vertical velocity, so they fall

		Entity::update(deltaTime); // Calls the base class update to handle animation and movement
	}

	void setVelocity(sf::Vector2f velocity) { m_velocity = velocity; }
	const sf::Vector2f& getVelocity() const { return m_velocity; }

	void setIsGrounded(bool grounded) { m_grounded = grounded; } // Sets the entity as "on the ground", doesn't actually move it
	bool isGrounded() const { return m_grounded; } // Check for whether the entity is on the ground
protected:
	sf::Vector2f m_velocity{ 0.f, 0.f };
	float m_gravity{ 980.f }; // Gravity affecting the entity
	bool m_grounded{ false }; // Whether the entity is on the ground
};