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

    auto collectable = std::make_unique<Collectable>(m_animationManager.getAnimation("playerWalk")); // ADD COIN SPRITE
    collectable->setPosition({ 0.f, 90.f });
    m_entities.push_back(std::move(collectable));

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

    const CollisionRectangle& playerHitbox = m_player->getHitbox(); // Sets up and updates the hitbox

    // Horizontal Updating
    m_player->move({ m_player->getVelocity().x * deltaTime, 0.f }); // Moves the player horizontally based on their velocity

 //   // Horizontal Collision
 //   for (const auto& entity : m_entities)
 //   {
 //       // Skips self-collision
 //       if (entity.get() == m_player) 
	//		continue;

	//	const CollisionRectangle& entityHitbox = entity->getHitbox();

 //       if (m_player->getHitbox().intersection(entityHitbox))
 //       {
 //           if (m_player->getVelocity().x > 0)
 //               m_player->setPosition({ entityHitbox.m_xPos - playerHitbox.m_width, m_player->getPosition().y });
 //           else if (m_player->getVelocity().x < 0)
	//			m_player->setPosition({ entityHitbox.m_xPos + entityHitbox.m_width, m_player->getPosition().y });
 //       }
 //   }

 //   // Vertical Updating
    m_player->move({ 0.f, m_player->getVelocity().y * deltaTime });
 //   m_player->setIsGrounded(false); // Resets grounded state each loop

	//// Vertical Collision
 //   for (const auto& entity : m_entities)
 //   {
 //       // Skips self-collision
 //       if (entity.get() == m_player)
 //           continue;

	//	const CollisionRectangle& entityHitbox = entity->getHitbox();

 //       if (m_player->getHitbox().intersection(entityHitbox))
 //       {
 //           if (m_player->getVelocity().y > 0)
 //           {
 //               m_player->setPosition({ m_player->getPosition().x, entityHitbox.m_yPos - playerHitbox.m_height});
 //               m_player->setIsGrounded(true);
 //               m_player->setYVelocity(0.f);
 //               
 //           }
 //           else if (m_player->getVelocity().y < 0)
 //           {
 //               m_player->setPosition({ m_player->getPosition().x, entityHitbox.m_yPos + entityHitbox.m_height});
 //               m_player->setYVelocity(0.f);                
 //           }
 //       }
 //   }

    // Deleting marked entities
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity) { return entity->getDestroy(); }), m_entities.end());

    // Trigger collider visualiser
    for (auto& pair : m_triggerColliders)
    {
        const CollisionRectangle& colliderRect = pair.second;
        m_triggerHitboxVisualiser.setPosition({ colliderRect.m_xPos, colliderRect.m_yPos });
        m_triggerHitboxVisualiser.setSize({ static_cast<float>(colliderRect.m_width), static_cast<float>(colliderRect.m_height) });
        m_triggerHitboxVisualiser.setFillColor(sf::Color(0, 255, 0, 100));
	}
}