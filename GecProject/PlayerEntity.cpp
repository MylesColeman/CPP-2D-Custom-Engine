#include "PlayerEntity.h"

void PlayerEntity::update()
{
	Entity::update(); // Calls the base class update to handle animation and movement
}

void PlayerEntity::handleInput(const std::vector<Actions>& actions)
{
	m_velocity = { 0.0f, 0.0f }; // Resets the velocity to zero each frame, so the player stops moving when no keys are pressed
	for (const Actions& action : actions) // Loops through all actions to handle multiple inputs
	{
		switch (action)
		{
		case Actions::eMoveUp:
			m_velocity.y = -2.0f; // Moves up by setting a negative y velocity
			break;
		case Actions::eMoveRight:
			m_velocity.x = 2.0f; // Moves right by setting a positive x velocity
			break;
		case Actions::eMoveDown:
			m_velocity.y = 2.0f; // Moves down by setting a positive y velocity
			break;
		case Actions::eMoveLeft:
			m_velocity.x = -2.0f; // Moves left by setting a negative x velocity
			break;
		default:
			break;
		}
	}
}
