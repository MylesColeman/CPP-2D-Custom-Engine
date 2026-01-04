#include "Simulation.h"

Simulation::Simulation(TextureManager& textureManager) :
    m_animationManager(textureManager)
{
    reset();
}

void Simulation::reset()
{
    m_player = nullptr;
    m_score = 0;
    m_levelComplete = false;

    m_inputManager.clearListeners();

    loadLevel("Data/Levels/Level1.txt");

    if (!m_player)
    {
        std::cout << "CRITICAL ERROR: Player not spawned!" << std::endl;
    }
}

bool Simulation::isGameOver() const
{
    if (!m_player) return true;
    return m_player->getHealth() <= 0;
}

// Updates the input manager with new inputs, loops through all entities and updates them, and handles the hitboxes and collisions
void Simulation::update(float deltaTime)
{
	// Store previous positions for interpolation - doneso before updating positions
    if (m_player)
        m_player->setPreviousPosition(m_player->getPosition());

    for (auto& entity : m_entities)
    {
        entity->setPreviousPosition(entity->getPosition());
    }

    for (auto& bullet : m_bulletPool)
    {
        if (bullet->isActive())
            bullet->setPreviousPosition(bullet->getPosition());
    }

	if (!m_player) return; // Safety check - incase player is null

    m_inputManager.update();

    sf::Vector2f shootDir;
    bool facingRight;

	// Handle shooting
    if (m_player->tryShoot(shootDir, facingRight))
    {
        // Adjust to gun height to better match player sprite (the gun)
        sf::Vector2f spawnPos = m_player->getPosition();

		// Offset for the gun position
        float xOffset = 2.5f;
        float yOffset = -0.5f;

		// Adjust spawn position based on facing direction
        if (facingRight)
            spawnPos.x += xOffset;
        else
            spawnPos.x -= xOffset;

		spawnPos.y += yOffset; // Adjust for gun height

        // Find the first sleeping bullet and fire it
        for (auto& bullet : m_bulletPool)
        {
            if (!bullet->isActive())
            {
				bullet->fire(spawnPos, shootDir * 250.f, false); // Multiplies direction by speed
                break; // We fired one, stop looking
            }
        }
    }

	// Enemy Shooting
    for (auto& entity : m_entities)
    {
        // Check if this entity is an Enemy
        Enemy* enemy = dynamic_cast<Enemy*>(entity.get());
        if (!enemy) continue; // Skip non-enemies

		sf::Vector2f shotDir; // Checks which direction to shoot in
		// Attempt to shoot
        if (enemy->tryShoot(shotDir))
        {
            // Determine spawn position (adjust for gun height)
            sf::Vector2f spawnPos = enemy->getPosition();
            
            // Offset for the gun position
            float xOffset = 2.5f;
            float yOffset = -0.5f;

            // Adjust spawn position based on facing direction
            if (facingRight)
                spawnPos.x += xOffset;
            else
                spawnPos.x -= xOffset;

            spawnPos.y += yOffset; // Adjust for gun height

            // Find a bullet to fire
            for (auto& bullet : m_bulletPool)
            {
				// Only fire if the bullet is inactive
                if (!bullet->isActive())
                {
                    bullet->fire(spawnPos, shotDir * 250.f, true);
                    break;
                }
            }
        }
    }

    // Loops through all entities and updates them
    for (auto& entity : m_entities)
    {
        entity->update(deltaTime);

		DynamicEntity* dynamicEntity = dynamic_cast<DynamicEntity*>(entity.get());

		if (dynamicEntity == nullptr) continue; // Skips if not dynamic, as only dynamic entities need collision handling

		// Edge Detection for Enemies
        Enemy* enemy = dynamic_cast<Enemy*>(entity.get());
		if (enemy && enemy->isGrounded() && std::abs(enemy->getSpeed()) > 0.1) // Only checks for enemies that are on the ground
        {
            sf::Vector2f velocity = enemy->getVelocity();
            CollisionRectangle enemyBox = enemy->getHitbox();

            // Create a small "sensor" box
            CollisionRectangle edgeSensor;
			edgeSensor.m_width = 4.f;  // Small width to ensure it only checks directly in front
			edgeSensor.m_height = 4.f; // Small height to just check below the feet
            edgeSensor.m_yPos = enemyBox.m_yPos + enemyBox.m_height; // Positioned at the feet

            // Place sensor to the Left or Right depending on movement
            if (velocity.x > 0) // Moving Right
                edgeSensor.m_xPos = enemyBox.m_xPos + enemyBox.m_width;
            else if (velocity.x < 0) // Moving Left
                edgeSensor.m_xPos = enemyBox.m_xPos - edgeSensor.m_width;

            bool groundFound = false;

            // Check if the sensor touches ANY floor tile
            for (const auto& floor : m_entities)
            {
				// Ignores self-collision & non-floor tiles
                if (floor->getType() != EntityType::Standard) continue;

				// Checks for intersection with the floor hitbox
                if (edgeSensor.intersection(floor->getHitbox()))
                {
                    groundFound = true;
					break; // No need to check further if ground is found
                }
            }

			// If no ground found, turn around
            if (!groundFound)
                enemy->turnAround(); // Reverses direction
        }

		sf::Vector2f velocity = dynamicEntity->getVelocity();

        // X
        dynamicEntity->move({ velocity.x * deltaTime, 0.f });
        dynamicEntity->syncHitbox();

		const CollisionRectangle& entityHitbox = dynamicEntity->getHitbox();

        for (const auto& wall : m_entities)
        {
			// Skips self, collectable, player and door collision
            if (wall.get() == dynamicEntity) continue;          
            if (wall->getType() == EntityType::Collectable) continue;
            if (wall->getType() == EntityType::Player) continue;
            if (wall->getType() == EntityType::Door) continue;
                
            // Creates a slimmer hitbox for wall checking
            CollisionRectangle wallCheck = entityHitbox;
            wallCheck.m_height -= 2.f;
            wallCheck.m_yPos += 1.f; // Centres it

            // Wall Collisions
            if (wallCheck.intersection(wall->getHitbox()))
            {
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

        // Y
        dynamicEntity->move({ 0.f, velocity.y * deltaTime });
        dynamicEntity->syncHitbox();
		dynamicEntity->setIsGrounded(false); // Resets grounded state each loop

        for (const auto& floor : m_entities)
        {
            // Skips self, collectable, player and door collision
            if (floor.get() == dynamicEntity) continue;
            if (floor->getType() == EntityType::Collectable) continue;
            if (floor->getType() == EntityType::Player) continue;
			if (floor->getType() == EntityType::Door) continue;

            // Creates a slimmer hitbox for wall checking
            CollisionRectangle floorCheck = entityHitbox;
            floorCheck.m_width -= 2.f;
            floorCheck.m_xPos += 1.f; // Centres it

            // Floor Collisions
            if (floorCheck.intersection(floor->getHitbox()))
            {
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

	// Bullet Collision - Is separate as bullets are not recognised as entities in the main entity vector
    for (auto& bullet : m_bulletPool)
    {
		if (!bullet->isActive()) continue; // Only checks active bullets

		bullet->update(deltaTime); // Ensures the bullet is deactivated after its lifetime
        bullet->move(bullet->getVelocity() * deltaTime);
        bullet->syncHitbox();

        // Check Collision with World (Walls/Floors)
        for (const auto& obstacle : m_entities)
        {
            // Ignore self types and collectables
            if (obstacle->getType() == EntityType::Collectable ||
                obstacle->getType() == EntityType::Bullet) continue;

			// Checks bullet type to determine what it can hit
			if (bullet->isEnemyBullet()) 
            {
                if (obstacle->getType() == EntityType::Enemy) continue; // Enemy bullets ignore enemies
            }
            else
                if (obstacle->getType() == EntityType::Player) continue; // Player bullets ignore player

            if (bullet->getHitbox().intersection(obstacle->getHitbox()))
            {
                bullet->deactivate(); // Collision detected, deactivate bullet

                // Handles the different bullet types
				if (bullet->isEnemyBullet()) // Enemy bullet
                {
					// Damages player upon being hit by enemy bullet
                    if (obstacle->getType() == EntityType::Player)
                        m_player->takeDamage(1);

                }
				else // Player bullet
                {
					// Destroys enemy upon being hit by player bullet
                    if (obstacle->getType() == EntityType::Enemy) 
                    {
                        Enemy* enemy = dynamic_cast<Enemy*>(obstacle.get());
                        if (enemy)
                        {
							enemy->takeDamage(1); // Deals 1 damage to the enemy

							// Check if their health is 0 or below - destroys them and adds 5 score if so
                            if (enemy->getHealth() <= 0)
                            {
                                obstacle->destroy();

								// Checks if the enemy was destroyed to add score
                                if (enemy->getDestroy())
                                {
                                    m_score += 5;
                                }
                            }
                        }
                    }
                }
                break;
            }
        }
    }

    const CollisionRectangle& playerHitbox = m_player->getHitbox();

	// Door Collision - Level Completion
    for (const auto& entity : m_entities)
    {
        if (entity->getType() == EntityType::Door)
        {
            if (playerHitbox.intersection(entity->getHitbox()))
            {
                float playerCenterX = playerHitbox.m_xPos + (playerHitbox.m_width / 2.f);

                float doorCenterX = entity->getHitbox().m_xPos + (entity->getHitbox().m_width / 2.f);

                // Calculate the distance between centers
                float diffX = std::abs(playerCenterX - doorCenterX);

				// If close enough to the center, mark level as complete - to simulate actually entering the door. Not just touching
                if (diffX < 4.0f) { m_levelComplete = true; }
            }
        }
    }

	// Collectable Collision
    for (auto& entity : m_entities)
    {
		// Marks collectables for destruction upon collision with player
        if (entity->getType() == EntityType::Collectable && playerHitbox.intersection(entity->getHitbox()))
        {
            m_score += 1; // Increments the score variable
            entity->destroy();
        }
    }

	// Trigger Collision
    for (auto& pair : m_triggerColliders)
    {
        // None currently implemented
	}

    // Deleting marked entities
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity) { return entity->getDestroy(); }), m_entities.end());
}

void Simulation::loadLevel(const std::string& filename)
{
	// Loads level from a text file
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to load level: " << filename << std::endl;
        return;
    }

	// Clears existing entities and bullets
    m_entities.clear();
    m_bulletPool.clear();

    // Bullet setup
    const StaticSprite& bullet = m_animationManager.getStaticSprite("bullet");
    // Pre-create a pool of bullets
    for (int i = 0; i < 20; ++i)
    {
        m_bulletPool.push_back(std::make_unique<Bullet>(bullet));
    }

    std::string line;
    int y = 0;
	int maxX = 0; // To calculate level width (for the level size variable, used by the camera)
    float tileSize = 18.f; // How large a single floor tile is

	// Reads each line from the file
    while (std::getline(file, line))
    {
		std::stringstream ss(line); // String stream for parsing
		std::string cell;
        int x = 0;

		// Separates each cell by commas
        while (std::getline(ss, cell, ','))
        {
			int tileId = std::stoi(cell); // Converts string to integer
			if (tileId != 0) // 0 represents empty space
            {
                createEntityFromId(tileId, x * tileSize, y * tileSize);
            }
			x++;
        }
        if (x > maxX) maxX = x;
        y++;
    }

	m_levelSize = { maxX * tileSize, y * tileSize }; // Sets level size based on loaded tiles

	// Ensures all enemies have reference to the player
    if (m_player)
    {
        for (auto& entity : m_entities)
        {
            // Check if it's an enemy
            if (entity->getType() == EntityType::Enemy)
            {
                // Dynamic cast to safely access Enemy-specific functions
                if (auto* enemy = dynamic_cast<Enemy*>(entity.get()))
                {
                    enemy->setTarget(m_player);
                }
            }
        }
    }
}

void Simulation::createEntityFromId(int id, float x, float y)
{
	// Offset to centre the entity in the tile
    float offset = 9.f;
    sf::Vector2f pos = { x + offset, y + offset };

	// Special Entities - Not tiles
	if (id >= 200) // 200 is an arbitrary cutoff for special entities
    {
		// Looks at the ID to determine which special entity to create
        switch (id)
        {
        case 999: // Player
        {
            // Only creates the player if it doesn't already exist
            if (!m_player)
            {
                auto player = std::make_unique<PlayerEntity>(m_animationManager);
                m_player = player.get();
                m_entities.push_back(std::move(player));
                m_inputManager.addListener(m_player);
            }
            m_player->setPosition(pos);
        }
        break;
        case 998: // Coin
        {
            auto coin = std::make_unique<Collectable>(m_animationManager.getAnimation("coin"));
            coin->setPosition(pos);
            m_entities.push_back(std::move(coin));
        }
        break;
        case 997: // Patrolling Enemy
        {
            auto enemy = std::make_unique<Enemy>(m_animationManager, 150.f); // 150.f patrol range
            enemy->setPosition(pos);
            m_entities.push_back(std::move(enemy));
        }
        break;
        case 996: // Stationary Enemy
        {
            auto enemy = std::make_unique<Enemy>(m_animationManager, 0.f); // 0.f patrol range - stationary
            enemy->setPosition(pos);
            m_entities.push_back(std::move(enemy));
        }
        break;
        case 995: // Door (Level Exit)
        {
            const StaticSprite& sprite = m_animationManager.getStaticSprite("door");
            auto door = std::make_unique<Door>(sprite);
            door->setPosition(pos);
            m_entities.push_back(std::move(door));
		}
        break;
        }
        return; // Entity created, exit function
    }

	// Tile Entities
    int tileIndex = id - 1;
	std::string tileName = "tile_" + std::to_string(tileIndex); // Generates the tile name based on the ID

	// Attempts to get the static sprite and create the tile entity (Preents crash on missing texture, incorrect ID)
    try
    {
        const StaticSprite& sprite = m_animationManager.getStaticSprite(tileName);

        auto tile = std::make_unique<Entity>(sprite);
        tile->setPosition(pos);
        m_entities.push_back(std::move(tile));
    }
	catch (const std::exception&) // Missing texture or invalid ID
    {
        std::cout << "WARNING: Level loading skipped invalid Tile ID: " << id << " (Missing texture: " << tileName << ")" << std::endl;
    }
}