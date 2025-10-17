#include "InputManager.h"

// Adds a listener for input events, so they can handle input events
void InputManager::addListener(IReceivesInput* listener)
{
	m_listeners.push_back(listener);
}

void InputManager::update()
{
	std::vector<Actions> actions;

	// Checks for key presses and adds them to to the actions vector - so the listeners can handle multiple inputs
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		actions.push_back(Actions::eMoveUp);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		actions.push_back(Actions::eMoveLeft);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		actions.push_back(Actions::eMoveDown);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		actions.push_back(Actions::eMoveRight);

	// Loops through all listeners and handles the input - if there are any
	for (IReceivesInput* listeners : m_listeners)
	{
		listeners->handleInput(actions);
	}
}