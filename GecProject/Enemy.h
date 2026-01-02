#pragma once
#include "DynamicEntity.h"
#include "PlayerEntity.h"

class Enemy : public DynamicEntity
{
public:
    explicit Enemy(const AnimationManager& animManager, float m_patrolRange = 150.f); // Uses the base class constructor

    void update(float deltaTime) override;
    
	void setTarget(PlayerEntity* player) { m_target = player; } // Sets the player as the target for the enemy to track

    bool tryShoot(sf::Vector2f& direction); // Returns whether the player's attempt to shoot was successful

    // Health management
    void takeDamage(int amount);
    int getHealth() const { return m_health; }
private:
	int m_health{ 2 }; // Enemy health

    bool canSeePlayer() const;

    // Animations
    const Animation* m_playerWalk{ nullptr };
    const Animation* m_playerStandingShot{ nullptr };

    PlayerEntity* m_target{ nullptr };

    enum class State { Patrolling, Attacking }; // Enemy States
	State m_state{ State::Patrolling }; // Current enemy state

    // Defines the enemy's vision range for detecting the player - goes off of the viewport to ensure the enemy can always
	// see the player when they are on screen
    float m_visionRangeX{ 150.f };
    float m_visionRangeY{ 180.f };

    float m_shootTimer{ 0.f };
    float m_shootCooldown{ 0.65f }; // Time between being able to shoot again

    float m_speed{ 50.f }; // Enemy movement speed
    float m_patrolRange{ 150.f }; // How far the enemy patrols from its starting position
    float m_startX{ 0.f }; // Sets the starting patrol position, to help determine how far the enemy has moved
	bool m_hasSetStartPos{ false }; // Whether the starting position has been set yet, to avoid resetting it every frame
};