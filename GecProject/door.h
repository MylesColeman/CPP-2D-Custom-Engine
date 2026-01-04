#pragma once
#include "Entity.h"

class Door : public Entity
{
public:
    explicit Door(const StaticSprite& sprite) : Entity(sprite, EntityType::Door) {}

    void update(float deltaTime) override 
    {
        Entity::update(deltaTime);
    };
};