#include "Simulation.h"

Simulation::Simulation(TextureManager& textureManager) :
    m_animationManager(textureManager)
{
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
    auto floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 0.f, 108.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
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
    floor->setPosition({ 54.f, 144.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 90.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 108.f, 72.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 108.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 126.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 144.f, 126.f });
    m_entities.push_back(std::move(floor));

    floor = std::make_unique<Entity>(m_animationManager.getStaticSprite("TopEdgelessFloor"));
    floor->setPosition({ 162.f, 126.f });
    m_entities.push_back(std::move(floor));
}

// Updates the input manager with new inputs, loops through all entities and updates them, and handles the hitboxes and collisions
void Simulation::update(float deltaTime)
{
    m_inputManager.update();

    // Loops through all entities and updates them
    for (auto& entity : m_entities)
        entity->update(deltaTime);

    // Horizontal Updating
    m_player->move({ m_player->getVelocity().x * deltaTime, 0.f }); // Moves the player horizontally based on their velocity
    m_player->syncHitbox(); // Hitbox gets synced after every movement
    const CollisionRectangle& playerHitbox = m_player->getHitbox(); // Sets up and updates the hitbox

    // Horizontal Collision
    for (const auto& entity : m_entities)
    {
        // Skips self-collision
        if (entity.get() == m_player) 
			continue;
        // Skips collectable collision, as its handled later
        if (entity->getType() == EntityType::Collectable)
			continue;

		const CollisionRectangle& entityHitbox = entity->getHitbox();
        // Creates a slimmer hitbox for wall checking
        CollisionRectangle wallCheck = playerHitbox; 
        wallCheck.m_height -= 2.f;
        wallCheck.m_yPos += 1.f; // Centres it

        if (wallCheck.intersection(entityHitbox))
        {
            // Checks for horizontal movement
            if (m_player->getVelocity().x > 0)
                m_player->setPosition({ entityHitbox.m_xPos - playerHitbox.m_width, m_player->getPosition().y });
            else if (m_player->getVelocity().x < 0)
				m_player->setPosition({ entityHitbox.m_xPos + entityHitbox.m_width, m_player->getPosition().y });

            m_player->syncHitbox(); // Ensures the hitbox is synced after the position change
            break;
        }
    }

    // Vertical Updating
    m_player->move({ 0.f, m_player->getVelocity().y * deltaTime });
    m_player->syncHitbox(); // Hitbox gets synced after every movement
    m_player->setIsGrounded(false); // Resets grounded state each loop

	// Vertical Collision
    for (const auto& entity : m_entities)
    {
        // Skips self-collision
        if (entity.get() == m_player)
            continue;
        // Skips collectable collision, as its handled later
        if (entity->getType() == EntityType::Collectable)
            continue;

		const CollisionRectangle& entityHitbox = entity->getHitbox();
        // Creates a slimmer hitbox for floor/ceiling checking
        CollisionRectangle floorCheck = m_player->getHitbox();
        floorCheck.m_width -= 2.f;
        floorCheck.m_xPos += 1.f; // Centres it


        if (floorCheck.intersection(entityHitbox))
        {
            // Checks for vertical movement
            if (m_player->getVelocity().y > 0)
            {
                m_player->setPosition({ m_player->getPosition().x, entityHitbox.m_yPos - playerHitbox.m_height});
                m_player->setIsGrounded(true); // Sets grounded state when landing on a surface
                m_player->setYVelocity(0.f);
                
            }
            else if (m_player->getVelocity().y < 0)
            {
                m_player->setPosition({ m_player->getPosition().x, entityHitbox.m_yPos + entityHitbox.m_height});
                m_player->setYVelocity(0.f);                
            }

            m_player->syncHitbox(); // Ensures the hitbox is synced after the position change
            break;
        }
    }

	// Collectable Collision
    for (auto& entity : m_entities)
    {
        // Skips redundant collisions
        if (entity.get() == m_player)
			continue;

        if (entity->getType() == EntityType::Collectable && playerHitbox.intersection(entity->getHitbox()))
            entity->destroy();
    }

	// Trigger Collision
    for (auto& pair : m_triggerColliders)
    {
        
	}

    // Deleting marked entities
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity) { return entity->getDestroy(); }), m_entities.end());
}