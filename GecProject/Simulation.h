#pragma once
#include "Entity.h"
#include "DynamicEntity.h"
#include "PlayerEntity.h"
#include "Collectable.h"
#include "Bullet.h"
#include "InputManager.h"
#include "CollisionRectangle.h"
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <string>

class Simulation
{
public:
    Simulation(TextureManager& textureManager);
    void update(float deltaTime); // Updates the input manager with new inputs, loops through all entities and updates them and, handles the hitboxes and collisions

    sf::Vector2f getLevelSize() const { return m_levelSize; }

    // A getter function for the entities for use in the graphics (for rendering)
    const std::vector<std::unique_ptr<Entity>>& getEntities() const { return m_entities; }

	const PlayerEntity* getPlayer() const { return m_player; } //  Getter for the player entity, for use in graphics

	// Colliders
    std::vector<CollisionRectangle> m_solidColliders;
    std::unordered_map<std::string, CollisionRectangle> m_triggerColliders;

	// A getter function for the bullets for use in the graphics (for rendering)
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const { return m_bulletPool; }

    // Debugging hitbox visualisers
    sf::RectangleShape m_triggerHitboxVisualiser;
private:
    AnimationManager m_animationManager;
    InputManager m_inputManager;

    std::vector<std::unique_ptr<Entity>> m_entities; // Scalable approach used for updating and rendering

    // For quicker access than looping through the vector
    PlayerEntity* m_player{ nullptr };

	sf::Vector2f m_levelSize{ 500.f, 500.f }; // Defines the size of the level. TEMPORARY SOLUTION

	std::vector<std::unique_ptr<Bullet>> m_bulletPool; // Defines all bullets in the simulation

    int m_score{ 0 }; // Keeps track of the player's score
};