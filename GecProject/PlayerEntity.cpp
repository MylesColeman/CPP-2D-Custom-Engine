#include "PlayerEntity.h"
#include <iostream>

PlayerEntity::PlayerEntity(const AnimationManager& animManager) : DynamicEntity(animManager.getAnimation("playerIdle"), EntityType::Player)
{
	m_playerIdle = &animManager.getAnimation("playerIdle");
	m_playerJump = &animManager.getAnimation("playerJump");
	m_playerJumpShot = &animManager.getAnimation("playerJumpShot");
	m_playerStandingShot = &animManager.getAnimation("playerStandingShot");
	m_playerWalk = &animManager.getAnimation("playerWalk");
	m_playerWalkShot = &animManager.getAnimation("playerWalkShot");
}

void PlayerEntity::update(float deltaTime)
{
	// Shoot Cooldown Timer
	if (m_shootCooldownTimer > 0.f)
		m_shootCooldownTimer -= deltaTime;

	// Sprite Flipper
	if (m_velocity.x < 0)
		this->flipSprite(true);
	else if (m_velocity.x > 0)
		this->flipSprite(false);

	// Animation Setter
	if (m_shootCooldownTimer > 0.f)
	{
		if (m_grounded)
		{
			if (m_velocity.x != 0)
			{
				if (m_animation != m_playerWalkShot)
					this->setAnimation(*m_playerWalkShot);
			}
			else
			{
				if (m_animation != m_playerStandingShot)
					this->setAnimation(*m_playerStandingShot);
			}
		}
		else
		{
			if (m_animation != m_playerJumpShot)
				this->setAnimation(*m_playerJumpShot);
		}
	}
	else
	{
		if (m_grounded)
		{
			if (m_velocity.x != 0)
			{
				if (m_animation != m_playerWalk)
					this->setAnimation(*m_playerWalk);
			}
			else
			{
				if (m_animation != m_playerIdle)
					this->setAnimation(*m_playerIdle);
			}
		}
		else
		{
			if (m_animation != m_playerJump)
				this->setAnimation(*m_playerJump);
		}
	}
	
	DynamicEntity::update(deltaTime); // Calls the base class update to handle animation and movement
}

void PlayerEntity::handleInput(const std::vector<Actions>& actions)
{
	m_velocity.x = 0.f; // Resets the velocity to zero each frame, so the player stops moving when no keys are pressed
	bool isJumping{ false }; // Whether the player is currently jumping, for the variable jump-height

	// Resets the looking up/down states each frame
	m_isLookingUp = false;
	m_isLookingDown = false;

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
			isJumping = true; // Sets the jumping flag to true
			break;
		case Actions::eLookUp:
			m_isLookingUp = true;
			break;
		case Actions::eLookDown:
			m_isLookingDown = true;
			break;
		case Actions::eShoot:
			m_wantsToShoot = true;
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

bool PlayerEntity::tryShoot(sf::Vector2f& direction, bool& facingRight)
{
	if (m_wantsToShoot && m_shootCooldownTimer <= 0.f)
	{
		m_shootCooldownTimer = m_shootCooldown; // Sets a cooldown of 0.5 seconds between shots
		m_wantsToShoot = false;

		float xDir = isFacingRight() ? 1.f : -1.f;
		float yDir = 0.f;

		if (m_isLookingUp) yDir = -1.f;
		if (m_isLookingDown) yDir = 1.f;

		direction = { xDir, yDir };
		facingRight = isFacingRight();

		return true;
	}

	return false; // Didn't shoot
}
