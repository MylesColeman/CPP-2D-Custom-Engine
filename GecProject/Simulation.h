#pragma once
#include "Entity.h"
#include "PlayerEntity.h"
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
    void update(); // Updates the input manager with new inputs, loops through all entities and updates them and, handles the hitboxes and collisions

    // A getter function for the entities for use in the graphics (for rendering)
    const std::vector<std::unique_ptr<Entity>>& getEntities() const
    {
        return m_entities;
    }

    std::vector<CollisionRectangle> m_solidColliders;
    std::map<std::string, CollisionRectangle> m_triggerColliders;
private:
    AnimationManager m_animationManager;
    InputManager m_inputManager;

    std::vector<std::unique_ptr<Entity>> m_entities; // Scalable approach used for updating and rendering

    // For quicker access than looping through the vector
    PlayerEntity* m_player{ nullptr };
    Entity* m_zombie{ nullptr };
};