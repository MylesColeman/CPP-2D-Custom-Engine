#include "PlayerEntity.h"
#include <iostream>

void PlayerEntity::update(float deltaTime)
{
	m_velocity.y += m_gravity * deltaTime; // Applies gravity to the player's vertical velocity, so they fall

	Entity::update(deltaTime); // Calls the base class update to handle animation and movement
}

void PlayerEntity::handleInput(const std::vector<Actions>& actions)
{
	m_velocity.x = 0.f; // Resets the velocity to zero each frame, so the player stops moving when no keys are pressed
	bool isJumping{ false }; // Whether the player is currently jumping, for the variable jump-height

	for (const Actions& action : actions) // Loops through all actions to handle multiple inputs
	{
		switch (action)
		{
		case Actions::eMoveRight:
			m_velocity.x = m_speed; // Moves right by setting a positive x velocity
			this->setAnimation(AnimationManager::getAnimation("playerWalk"));
			break;
		case Actions::eMoveLeft:
			m_velocity.x = -m_speed; // Moves left by setting a negative x velocity
			break;
		case Actions::eJump:
			isJumping = true; // Sets the jumping flag to true
			break;
		default:
			break;
		}
	}

	if (m_grounded && isJumping && !m_wasJumping)
	{
		m_velocity.y = m_jumpHeight; // Sets a negative y velocity to make the player jump
		m_grounded = false;
	}

	if (m_velocity.y < 0 && !isJumping)
		m_velocity.y *= 0.5f;

	m_wasJumping = isJumping; // Updates the wasJumping flag for the next frame
}