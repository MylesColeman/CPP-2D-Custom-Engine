#include "SimulationSystem.h"

SimulationSystem::SimulationSystem(TextureManager& textureManager) :
    m_animationManager(textureManager)
{
    auto player = std::make_unique<PlayerEntity>(m_animationManager.getAnimation("zombieWalk"));
    m_player = player.get();
    m_entities.push_back(std::move(player));
    m_inputManager.addListener(m_player);

    auto zombie = std::make_unique<Entity>(m_animationManager.getAnimation("zombieIdle"));
    m_zombie = zombie.get();
    m_entities.push_back(std::move(zombie));
}

// Updates the input manager with new inputs, loops through all entities and updates them, and handles the hitboxes and collisions
void SimulationSystem::update()
{
    m_inputManager.update();

    // Loops through all entities and updates them
    for (auto& entity : m_entities)
        entity->update();

    // Sets up and updates the hitboxes
    const CollisionRectangle& playerHitbox = m_player->getHitbox();
    const CollisionRectangle& zombieHitbox = m_zombie->getHitbox();

    // Collision detection
    if (playerHitbox.intersection(zombieHitbox))
        std::cout << "Collision Detected!" << std::endl;
}