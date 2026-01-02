#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const AnimationManager& animManager, float patrolRange)
    : DynamicEntity(animManager.getAnimation("playerWalk"), EntityType::Enemy), m_patrolRange(patrolRange)
{
    m_playerWalk = &animManager.getAnimation("playerWalk");
    m_playerStandingShot = &animManager.getAnimation("playerStandingShot");

	m_velocity.x = -m_speed; // Sets the enemy to move left initially, as this will typically lead them towards the player
}

void Enemy::update(float deltaTime)
{
    {
        if (!m_hasSetStartPos)
        {
            m_startX = getPosition().x;
            m_hasSetStartPos = true;
        }

        // Shoot Cooldown Timer
        if (m_shootTimer > 0.f)
            m_shootTimer -= deltaTime;

        // State Management
        if (canSeePlayer()) // If player is visible, attack
        {
            m_state = State::Attacking;
        }
        else // Otherwise patrol
        {
            m_state = State::Patrolling;
        }

		// State Behaviors
		if (m_state == State::Attacking) // Attack Behavior
        {
            m_velocity.x = 0.f; // Stops the enemy from moving, whilst attacking

            // Change Animation to Shooting
            if (m_animation != m_playerStandingShot)
                setAnimation(*m_playerStandingShot);
        }
		else // Patrol Behavior
        {
            // Change Animationto Walking
            if (m_animation != m_playerWalk)
                setAnimation(*m_playerWalk);

            // Restore velocity based on which way we were trying to go
            // (This logic ensures we don't get stuck standing still after an attack)
            if (m_velocity.x == 0.f)
                m_velocity.x = (m_speed > 0) ? std::abs(m_speed) : -std::abs(m_speed);

            if (std::abs(m_velocity.x) < 0.1f) // Enemy has stopped due to a collision, turn around
            {
                m_speed = -m_speed; // Reverse direction
                m_velocity.x = m_speed; // Apply new velocity
            }
            else if (m_velocity.x > 0 && getPosition().x > m_startX + m_patrolRange) // Moving right and exceeded patrol range
            {
                m_speed = -std::abs(m_speed); // Ensure speed is negative
                m_velocity.x = m_speed; // Apply new velocity
            }
            else if (m_velocity.x < 0 && getPosition().x < m_startX - m_patrolRange) // Moving left and exceeded patrol range
            {
                m_speed = std::abs(m_speed); // Ensure speed is positive
                m_velocity.x = m_speed; // Apply new velocity
            }
            else // No collision and inside patrol range
                m_velocity.x = (m_speed > 0) ? std::abs(m_speed) : -std::abs(m_speed); // Maintain current direction

            // Sprite Flipper
            if (m_velocity.x < 0)
                this->flipSprite(true);
            else if (m_velocity.x > 0)
                this->flipSprite(false);
        }

        // Turn Red to distinguish from Player!
        this->setColor(sf::Color::Red);

        // Call parent update to handle animation ticks
        DynamicEntity::update(deltaTime);
    }
}

// Attempts to shoot a projectile, returns true if successful
bool Enemy::tryShoot(sf::Vector2f& direction)
{
    // Checks whether the player wants to shoot and if the cooldown timer has elapsed
    if (m_state == State::Attacking && m_shootTimer <= 0.f)
    {
        m_shootTimer = m_shootCooldown; // Sets a cooldown of 0.5 seconds between shots

        sf::Vector2f myPos = getPosition();
        myPos.x += 6.5f; // Adjusts to shoot from the gun height
		myPos.y -= 10.f; // Adjusts to shoot from the gun height

		sf::Vector2f targetPos = m_target->getPosition(); // Player position used for comparison
		sf::Vector2f difference = targetPos - myPos; // Difference vector from enemy to player

		// Calculate the length of the difference vector, using square root as its more accurate for normalisation
        float length = std::sqrt(difference.x * difference.x + difference.y * difference.y); 

		// Normalises the direction vector
        if (length != 0)
			direction = difference / length; // Normalised direction vector towards player
        else
        {
			float dirX = m_flipped ? -1.f : 1.f; // Shoot left if flipped, right otherwise
			direction = { dirX, 0.f }; // Default direction
        }

        return true;
    }
    return false; // Didn't shoot
}

bool Enemy::canSeePlayer() const
{
    if (!m_target) return false; // No target to see

	sf::Vector2f myPos = getPosition(); // Enemy position used for comparison
	sf::Vector2f targetPos = m_target->getPosition(); // Player position used for comparison

    // Check Horizontal Distance
    if (std::abs(targetPos.x - myPos.x) > m_visionRangeX) return false; // Too far away

    // Check Vertical Distance
    if (std::abs(targetPos.y - myPos.y) > m_visionRangeY) return false; // Too high/low

    // Check Facing Direction
    // If I am facing Left (flipped), player must be to the Left (target < my)
    bool amIFacingLeft = m_flipped;
    bool isPlayerLeft = (targetPos.x < myPos.x);

	// If facing the player, can see them
    if (amIFacingLeft && isPlayerLeft) return true;
    if (!amIFacingLeft && !isPlayerLeft) return true;

    return false; // Player is behind the enemy
}
