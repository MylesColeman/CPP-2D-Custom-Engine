#include "PlayerEntity.h"
#include <iostream>

void PlayerEntity::update(float deltaTime)
{
	if (!m_grounded)
		m_velocity.y += m_gravity * deltaTime; // Applies gravity to the player's vertical velocity, so they fall

	Entity::update(deltaTime); // Calls the base class update to handle animation and movement
	//std::cout << m_velocity.y << std::endl;
}

void PlayerEntity::handleInput(const std::vector<Actions>& actions)
{
	m_velocity.x = 0.f; // Resets the velocity to zero each frame, so the player stops moving when no keys are pressed

	for (const Actions& action : actions) // Loops through all actions to handle multiple inputs
	{
		switch (action)
		{
		case Actions::eMoveRight:
			m_velocity.x = m_speed; // Moves right by setting a positive x velocity
			break;
		case Actions::eMoveLeft:
			m_velocity.x = -m_speed; // Moves left by setting a negative x velocity
			break;
		case Actions::eJump:
			// Jump logic would go here
			m_velocity.y = -m_speed;
			break;
		default:
			break;
		}
	}
}