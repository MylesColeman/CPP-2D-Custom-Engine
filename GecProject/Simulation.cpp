#include "Simulation.h"

Simulation::Simulation(TextureManager& textureManager) :
    m_animationManager(textureManager)
{
    // Adding entities to the entity vector, and setting up the animations for them
	// Player
    auto player = std::make_unique<PlayerEntity>(m_animationManager.getAnimation("zombieWalk"));
    m_player = player.get();
    m_entities.push_back(std::move(player));
    m_player->setPosition({ 0.f, 0.f });
    m_inputManager.addListener(m_player);

    // Zombie
    auto zombie = std::make_unique<Entity>(m_animationManager.getAnimation("zombieIdle"));
    m_zombie = zombie.get();
    m_entities.push_back(std::move(zombie));
	m_zombie->setPosition({ 600.f, 500.f });

    // Creating collision rectangles
    // Event Trigger Test
    m_triggerColliders["TestTrigger"] = CollisionRectangle(0.f, 0.f, 50, 50);
	m_triggerColliders["AnotherTestTrigger"] = CollisionRectangle(750.f, 650.f, 50, 50);
}

// Updates the input manager with new inputs, loops through all entities and updates them, and handles the hitboxes and collisions
void Simulation::update()
{
    m_inputManager.update();

    sf::Vector2f previousPlayerPos = m_player->getPosition();

    // Loops through all entities and updates them
    for (auto& entity : m_entities)
        entity->update();

    // Sets up and updates the hitboxes
    const CollisionRectangle& playerHitbox = m_player->getHitbox();
    const CollisionRectangle& zombieHitbox = m_zombie->getHitbox();

    // Collision detection
    // Collisions between entities
    for (const auto& entity : m_entities)
    {
        if (playerHitbox.intersection(entity->getHitbox()) && entity.get() != m_player)
        {
            std::cout << "Player collided with another entity!" << std::endl;
            m_player->setPosition(previousPlayerPos);
		}
    }

    // Collisions with solid colliders
    for (const auto& solidCollider : m_solidColliders)
    {
        if (playerHitbox.intersection(solidCollider))
            m_player->setPosition(previousPlayerPos);
    }

    // Collisions with trigger boxes
    for (const auto& pair : m_triggerColliders)
    {
        const std::string& triggerName = pair.first;
        const CollisionRectangle& colliderRect = pair.second;
        if (playerHitbox.intersection(colliderRect))
        {
			if (triggerName == "TestTrigger")
                std::cout << "Player triggered event: " << triggerName << std::endl;
            if (triggerName == "AnotherTestTrigger")
                std::cout << "Player triggered a different event: " << triggerName << std::endl;
        }
    }
}