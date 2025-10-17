#pragma once
#include "Entity.h"
#include "IReceivesInput.h"
class PlayerEntity : public Entity, public IReceivesInput
{
public:
	explicit PlayerEntity(const Animation& animation) : Entity(animation) {} // Uses the base class constructor

	void update() override;
	void handleInput(const std::vector<Actions>& actions);
};