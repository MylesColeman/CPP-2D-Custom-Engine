#pragma once
#include "Simulation.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Graphics
{
public:
	Graphics();
	void display(); // Will be called in main, running all the graphics/display logic
private:
	void windowEvents(); // Handles the windows events/interactions, movement, editing and closing
	void resizeView(const sf::Window& window, sf::View& view); // Resizes the view when the window is resized
	void update(float deltaTime); // Handles the updating of the simulation, which in turn handles the updating of entities (e.g. animations & movement). Also updates the ImGui and the FPS counter
	void render(); // The actual logic behind displaying the sprite to the window

	sf::RenderWindow m_window;
	sf::View m_gameView;

	TextureManager m_textureManager; // Passed into the simulation to be used by the animation manager
	Simulation m_simulation;

	sf::Clock m_uiDeltaClock; // Used by ImGui for updating
	sf::Clock m_deltaClock; // Used for calculating delta time between frames

	// Used for the physics update loop
	float m_accumulator{ 0.f }; // Accumulates delta time
	const float m_fixedTimestep{ 1.f / 60.f }; // Fixed timestep of 60 FPS, ensuring consistent physics updates

	// FPS counter variables
	sf::Clock m_frameClock;
	int m_frameCount{ 0 };
	float m_fps{ 0.0f };
};