#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "IReceivesInput.h"

class InputManager
{
private:
	std::vector<IReceivesInput*> m_listeners;
public:
	// Singleton pattern to ensure only one instance of InputManager exists
	static InputManager& getInstance()
	{
		static InputManager manager;

		return manager;
	}

	void addListener(IReceivesInput* listener);
	void update();
};