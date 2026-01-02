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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		actions.push_back(Actions::eMoveLeft);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		actions.push_back(Actions::eMoveRight);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		actions.push_back(Actions::eJump);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
		actions.push_back(Actions::eShoot);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		actions.push_back(Actions::eLookUp);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		actions.push_back(Actions::eLookDown);

	// Loops through all listeners and handles the input - if there are any
	for (IReceivesInput* listeners : m_listeners)
	{
		listeners->handleInput(actions);
	}
}