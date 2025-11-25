#pragma once
#include "Entity.h"
#include "IReceivesInput.h"
class PlayerEntity : public Entity, public IReceivesInput
{
public:
	explicit PlayerEntity(const Animation& animation) : Entity(animation, EntityType::Player) {} // Uses the base class constructor

	void update(float deltaTime) override;
	void handleInput(const std::vector<Actions>& actions);

	void setIsGrounded(bool grounded) { m_grounded = grounded; } 
	void setYVelocity(float YVelocity) { m_velocity.y = YVelocity; }
	const sf::Vector2f& getVelocity() const { return m_velocity; }
private:
	float m_speed{ 75.f }; // Defines the speed of the player
	float m_jumpHeight{ -350.f }; // Defines the jump height of the player

	float m_gravity{ 980.f }; // Gravity affecting the player
	bool m_grounded{ false }; // Whether the player is on the ground
};