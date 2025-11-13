#pragma once
#include "Entity.h"
class Collectable : public Entity
{
public:
	explicit Collectable(const Animation& animation) : Entity(animation, EntityType::Collectable) {}

	void update(float deltaTime) override;
};