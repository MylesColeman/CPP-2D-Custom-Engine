#include "Simulation.h"
#include "DynamicEntity.h"
#include "Bullet.h"

Simulation::Simulation(TextureManager& textureManager) :
    m_animationManager(textureManager)
{
    m_levelSize = { 500.f, 500.f };

    // Adding entities to the entity vector, and setting up the animations for them
	// Player
    auto player = std::make_unique<PlayerEntity>(m_animationManager);
    m_player = player.get();
    m_entities.push_back(std::move(player));
    m_player->setPosition({ 25.f, 50.f });
    m_inputManager.addListener(m_player);

    auto collectable = std::make_unique<Collectable>(m_animationManager.getAnimation("playerWalk")); // ADD COIN SPRITE
    collectable->setPosition({ 0.f, 90.f });
    m_entities.push_back(std::move(collectable));

    // Static Sprite
    // Floor
    float groundLevel = 126.f;
    float tileWidth = 18.f;

    // Generate a long floor (55 tiles * 18 width = ~1000 pixels)
    for (int i = 0; i < 55; ++i)
    {
        auto floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));

        floor->setPosition({ i * tileWidth, groundLevel });

        m_entities.push_back(std::move(floor));
    }
}

// Updates the input manager with new inputs, loops through all entities and updates them, and handles the hitboxes and collisions
void Simulation::update(float deltaTime)
{
    m_inputManager.update();

    sf::Vector2f shootDir;
    bool facingRight;

    if (m_player->tryShoot(shootDir, facingRight))
    {
        // Create Bullet
        const StaticSprite& bulletSprite = m_animationManager.getStaticSprite("bulletSprite");

        auto bullet = std::make_unique<Bullet>(bulletSprite, shootDir * 250.f);

        sf::Vector2f spawnPos = m_player->getPosition();
        spawnPos.y -= 5.f;
        bullet->setPosition(spawnPos);

        m_newEntities.push_back(std::move(bullet));
    }

    // Loops through all entities and updates them
    for (auto& entity : m_entities)
    {
        entity->update(deltaTime);

		DynamicEntity* dynamicEntity = dynamic_cast<DynamicEntity*>(entity.get());

        if (dynamicEntity == nullptr) continue; // Skips if not dynamic

		sf::Vector2f velocity = dynamicEntity->getVelocity();

        // X
        dynamicEntity->move({ velocity.x * deltaTime, 0.f });
        dynamicEntity->syncHitbox();

		const CollisionRectangle& entityHitbox = dynamicEntity->getHitbox();

        for (const auto& wall : m_entities)
        {
			// Skips self-collision & collectable collision
            if (wall.get() == dynamicEntity) continue;          
            if (wall->getType() == EntityType::Collectable) continue;
            if (wall->getType() == EntityType::Player) continue;
            if (wall->getType() == EntityType::Bullet) continue;
                
            // Creates a slimmer hitbox for wall checking
            CollisionRectangle wallCheck = entityHitbox;
            wallCheck.m_height -= 2.f;
            wallCheck.m_yPos += 1.f; // Centres it

            // Wall Collisions
            if (wallCheck.intersection(wall->getHitbox()))
            {
                if (dynamicEntity->getType() == EntityType::Bullet)
                {
                    dynamicEntity->destroy();
                    break; // Stop checking other walls, bullet is dead
                }

                sf::Vector2f pos = dynamicEntity->getPosition();
                float halfWidth = entityHitbox.m_width / 2.f;

                if (velocity.x > 0) // Moving Right
                {
                    float targetLeft = wall->getHitbox().m_xPos - entityHitbox.m_width;
                    pos.x = targetLeft + halfWidth;
                }
                else if (velocity.x < 0) // Moving Left
                {
                    float targetLeft = wall->getHitbox().m_xPos + wall->getHitbox().m_width;
                    pos.x = targetLeft + halfWidth;
                }

                dynamicEntity->setPosition(pos);
                dynamicEntity->syncHitbox();
            }
        }

        if (dynamicEntity->getDestroy()) continue; // If the bullet's already been destroyed, skip further checks

        // Y
        dynamicEntity->move({ 0.f, velocity.y * deltaTime });
        dynamicEntity->syncHitbox();
		dynamicEntity->setIsGrounded(false); // Resets grounded state each loop

        for (const auto& floor : m_entities)
        {
            // Skips self-collision & collectable collision
            if (floor.get() == dynamicEntity) continue;
            if (floor->getType() == EntityType::Collectable) continue;
            if (floor->getType() == EntityType::Player) continue;
            if (floor->getType() == EntityType::Bullet) continue;

            // Creates a slimmer hitbox for wall checking
            CollisionRectangle floorCheck = entityHitbox;
            floorCheck.m_width -= 2.f;
            floorCheck.m_xPos += 1.f; // Centres it

            // Floor Collisions
            if (floorCheck.intersection(floor->getHitbox()))
            {
                if (dynamicEntity->getType() == EntityType::Bullet)
                {
                    dynamicEntity->destroy();
					break; // Stop checking other floors, bullet is dead
                }

                sf::Vector2f pos = dynamicEntity->getPosition();
                float halfHeight = entityHitbox.m_height / 2.f;

                if (velocity.y > 0) // Moving Down (Falling)
                {
                    float targetTop = floor->getHitbox().m_yPos - entityHitbox.m_height;
                    pos.y = targetTop + halfHeight;

                    dynamicEntity->setIsGrounded(true);

                    // Stop falling
                    dynamicEntity->setVelocity({ velocity.x, 0.f });
                }
                else if (velocity.y < 0) // Moving Up (Jumping)
                {
                    float targetTop = floor->getHitbox().m_yPos + floor->getHitbox().m_height;
                    pos.y = targetTop + halfHeight;
         
                    // Stop rising
                    dynamicEntity->setVelocity({ velocity.x, 0.f });
                }

                dynamicEntity->setPosition(pos);
                dynamicEntity->syncHitbox();
            }
        }
    }

    const CollisionRectangle& playerHitbox = m_player->getHitbox();

	// Collectable Collision
    for (auto& entity : m_entities)
    {
        if (entity->getType() == EntityType::Collectable && playerHitbox.intersection(entity->getHitbox()))
            entity->destroy();
    }

	// Trigger Collision
    for (auto& pair : m_triggerColliders)
    {
        // None currently implemented
	}

    if (!m_newEntities.empty())
    {
        for (auto& newEnt : m_newEntities)
        {
            m_entities.push_back(std::move(newEnt));
        }
        m_newEntities.clear();
    }

    // Deleting marked entities
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity) { return entity->getDestroy(); }), m_entities.end());
}