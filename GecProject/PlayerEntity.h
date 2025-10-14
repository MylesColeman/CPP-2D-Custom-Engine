#pragma once
#include "Entity.h"
#include "IReceivesInput.h"
class PlayerEntity :
    public Entity
{
public:
	void update() override;
	void handleInput(const std::vector<Actions>& actions);
};