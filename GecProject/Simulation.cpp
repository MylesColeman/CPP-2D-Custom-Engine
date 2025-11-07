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

    // Static Sprite
    // Floor
	auto floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("WalledFloor"));
    floor->setPosition({ 200.f, 200.f });
    m_entities.push_back(std::move(floor));
    
    // Creating collision rectangles
    // Event Trigger Test
    m_triggerColliders["TestTrigger"] = CollisionRectangle(0.f, 0.f, 50, 50);
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

    // Collision detection
    // Collisions between entities
    for (const auto& entity : m_entities)
    {
        if (playerHitbox.intersection(entity->getHitbox()) && entity.get() != m_player)
            m_player->setPosition(previousPlayerPos);
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
        }
    }

	m_playerHitboxVisualiser.setPosition({ playerHitbox.m_xPos, playerHitbox.m_yPos });
	m_playerHitboxVisualiser.setSize({ static_cast<float>(playerHitbox.m_width), static_cast<float>(playerHitbox.m_height) });
	m_playerHitboxVisualiser.setFillColor(sf::Color(255, 0, 0, 100));

    for (auto& pair : m_triggerColliders)
    {
        const CollisionRectangle& colliderRect = pair.second;
        m_triggerHitboxVisualiser.setPosition({ colliderRect.m_xPos, colliderRect.m_yPos });
        m_triggerHitboxVisualiser.setSize({ static_cast<float>(colliderRect.m_width), static_cast<float>(colliderRect.m_height) });
        m_triggerHitboxVisualiser.setFillColor(sf::Color(0, 255, 0, 100));
	}
}