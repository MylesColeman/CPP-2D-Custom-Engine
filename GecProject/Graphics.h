#pragma once
#include "TextureManager.h"
#include "AnimationManager.h"
#include "SpriteAnimator.h"
#include <SFML/Graphics.hpp>

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
	AnimationManager m_animationManager;
	SpriteAnimator m_zombie;

	// Clock required by ImGui
	sf::Clock m_uiDeltaClock;
	// Used by ImGUI dropdown (combo) box
	const char* m_items[4] = { "Idle", "Walk", "Attack", "Death" };
	const char* m_current_item = "Idle";

	sf::Clock m_frameClock;
	int m_frameCount{ 0 };
	float m_fps{ 0.0f };

};