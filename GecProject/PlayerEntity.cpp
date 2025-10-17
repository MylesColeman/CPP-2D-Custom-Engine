#include "PlayerEntity.h"
#include <iostream>

void PlayerEntity::update()
{
	Entity::update(); // Calls the base class update to handle animation and movement
}

void PlayerEntity::handleInput(const std::vector<Actions>& actions)
{
	m_velocity = { 0.0f, 0.0f }; // Resets the velocity to zero each frame, so the player stops moving when no keys are pressed
	float speed = 0.05f; // Defines the speed of the player

	for (const Actions& action : actions) // Loops through all actions to handle multiple inputs
	{
		switch (action)
		{
		case Actions::eMoveUp:
			m_velocity.y = -speed; // Moves up by setting a negative y velocity
			break;
		case Actions::eMoveRight:
			m_velocity.x = speed; // Moves right by setting a positive x velocity
			break;
		case Actions::eMoveDown:
			m_velocity.y = speed; // Moves down by setting a positive y velocity
			break;
		case Actions::eMoveLeft:
			m_velocity.x = -speed; // Moves left by setting a negative x velocity
			break;
		default:
			break;
		}
	}
}