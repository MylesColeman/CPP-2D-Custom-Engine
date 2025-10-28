#pragma once
#include "Entity.h"
#include "PlayerEntity.h"
#include "InputManager.h"

class SimulationSystem
{
public:
	void update();
private:
	AnimationManager m_animationManager;

	InputManager m_inputManager;

	Entity m_zombie;
	PlayerEntity m_player;
};