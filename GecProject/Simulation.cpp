#include "Simulation.h"

Simulation::Simulation(TextureManager& textureManager) :
    m_animationManager(textureManager)
{
    // Adding entities to the entity vector, and setting up the animations for them
	// Player
    auto player = std::make_unique<PlayerEntity>(m_animationManager.getAnimation("playerIdle"));
    m_player = player.get();
    m_entities.push_back(std::move(player));
    m_player->setPosition({ 25.f, 50.f });
    m_inputManager.addListener(m_player);

    // Static Sprite
    // Floor
	auto floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 0.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 18.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 36.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 54.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 0.f, 108.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 54.f, 144.f });
    m_entities.push_back(std::move(floor));
    
    // Creating collision rectangles
    // Event Trigger Test
    m_triggerColliders["TestTrigger"] = CollisionRectangle(0.f, 0.f, 50, 50);
}

// Updates the input manager with new inputs, loops through all entities and updates them, and handles the hitboxes and collisions
void Simulation::update(float deltaTime)
{
    m_inputManager.update();

    // Loops through all entities and updates them
    for (auto& entity : m_entities)
        entity->update(deltaTime);

    // Sets up and updates the hitboxes
    const CollisionRectangle& playerHitbox = m_player->getHitbox();

    // Collision detection
    // Collisions between entities
    m_player->setIsGrounded(false); // Resets to false - so that gravity can continue
    for (const auto& entity : m_entities)
    {
        const CollisionRectangle& entityHitbox = entity->getHitbox();
        if (playerHitbox.intersection(entityHitbox) && entity.get() != m_player)
        {
            // Checks if the player is falling - if the player is falling and collided, they collided with the top of an entity
            if (m_player->getVelocity().y > 0)
            {
                m_player->setIsGrounded(true);
                m_player->setYVelocity(0.f);
                
                m_player->setPosition({ playerHitbox.m_xPos, entityHitbox.m_yPos - playerHitbox.m_height - 0.1f });
            }

            float playerCentreY = playerHitbox.m_yPos + (playerHitbox.m_height / 2.f);

            if (playerCentreY > entityHitbox.m_yPos && playerCentreY < (entityHitbox.m_yPos + entityHitbox.m_height))
            {
                if (m_player->getVelocity().x > 0) // Moving right collision
                    m_player->setPosition({ entityHitbox.m_xPos - playerHitbox.m_width, playerHitbox.m_yPos });
                else if (m_player->getVelocity().x < 0) // Moving left collision
                    m_player->setPosition({ entityHitbox.m_xPos + entityHitbox.m_width, playerHitbox.m_yPos });
            }
        }
    }

    // Collisions with solid colliders
    for (const auto& solidCollider : m_solidColliders)
    {
        /*if (playerHitbox.intersection(solidCollider))
            m_player->setPosition(previousPlayerPos);*/ //DO LATER -------------------------------------
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

    for (auto& pair : m_triggerColliders)
    {
        const CollisionRectangle& colliderRect = pair.second;
        m_triggerHitboxVisualiser.setPosition({ colliderRect.m_xPos, colliderRect.m_yPos });
        m_triggerHitboxVisualiser.setSize({ static_cast<float>(colliderRect.m_width), static_cast<float>(colliderRect.m_height) });
        m_triggerHitboxVisualiser.setFillColor(sf::Color(0, 255, 0, 100));
	}
}