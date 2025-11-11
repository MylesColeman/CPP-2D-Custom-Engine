#pragma once
#include "Entity.h"
#include "IReceivesInput.h"
class PlayerEntity : public Entity, public IReceivesInput
{
public:
	explicit PlayerEntity(const Animation& animation) : Entity(animation) {} // Uses the base class constructor

	void update(float deltaTime) override;
	void handleInput(const std::vector<Actions>& actions);
private:
	float m_speed{ 100.f }; // Defines the speed of the player
};