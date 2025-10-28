#pragma once
#include "SimulationSystem.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Graphics
{
public:
	Graphics();
	void display(); // Will be called in main, running all the graphics/display logic
private:
	void windowEvents(); // Handles the windows events/interactions, movement, editing and closing
	void update(); // Handles the updating of sprites/textures/animations
	void render(); // The actual logic behind displaying the sprite to the window

	sf::RenderWindow m_window;
	TextureManager m_textureManager;
	SimulationSystem m_simulation;

	sf::Clock m_uiDeltaClock; // Used by ImGui for updating

	sf::Clock m_frameClock;
	int m_frameCount{ 0 };
	float m_fps{ 0.0f };
};