#pragma once
#include "Entity.h"
#include "IReceivesInput.h"
class PlayerEntity : public Entity, public IReceivesInput
{
public:
	explicit PlayerEntity(const AnimationManager& animManager); // Uses the base class constructor

	void update(float deltaTime) override;
	void handleInput(const std::vector<Actions>& actions);

	void setIsGrounded(bool grounded) { m_grounded = grounded; } 
	void setYVelocity(float YVelocity) { m_velocity.y = YVelocity; }
	const sf::Vector2f& getVelocity() const { return m_velocity; }
private:
	float m_speed{ 75.f }; // Defines the speed of the player
	float m_jumpHeight{ -350.f }; // Defines the jump height of the player
	bool m_wasJumping{ false }; // Whether the player was jumping in the last frame

	float m_gravity{ 980.f }; // Gravity affecting the player
	bool m_grounded{ true }; // Whether the player is on the ground

	const Animation* m_playerIdle{ nullptr };
	const Animation* m_playerJump{ nullptr };
	const Animation* m_playerJumpShot{ nullptr };
	const Animation* m_playerStandingShot{ nullptr };
	const Animation* m_playerWalk{ nullptr };
	const Animation* m_playerWalkShot{ nullptr };
};