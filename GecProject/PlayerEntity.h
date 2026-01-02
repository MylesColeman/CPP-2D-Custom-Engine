#pragma once
#include "DynamicEntity.h"
#include "IReceivesInput.h"
class PlayerEntity : public DynamicEntity, public IReceivesInput
{
public:
	explicit PlayerEntity(const AnimationManager& animManager); // Uses the base class constructor

	void update(float deltaTime) override;
	void handleInput(const std::vector<Actions>& actions); // Implements the input handling from IReceivesInput

	bool tryShoot(sf::Vector2f& direction, bool& facingRight); // Returns whether the player's attempt to shoot was successful
	bool isFacingRight() const { return !m_flipped; } // Used to determine which way the player is facing for shooting
private:
	float m_speed{ 75.f }; // Defines the speed of the player
	float m_jumpHeight{ -350.f }; // Defines the jump height of the player
	bool m_wasJumping{ false }; // Whether the player was jumping in the last frame

	// Animations
	const Animation* m_playerIdle{ nullptr };
	const Animation* m_playerJump{ nullptr };
	const Animation* m_playerJumpShot{ nullptr };
	const Animation* m_playerStandingShot{ nullptr };
	const Animation* m_playerWalk{ nullptr };
	const Animation* m_playerWalkShot{ nullptr };

	// Used to determine which angle to shoot
	bool m_isLookingUp{ false };
	bool m_isLookingDown{ false };
	bool m_wantsToShoot{ false };

	float m_shootCooldownTimer{ 0.f };
	float m_shootCooldown{ 0.5f }; // Time between being able to shoot again
};