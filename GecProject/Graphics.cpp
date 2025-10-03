#include "Graphics.h"

// Used to set up the textures and animations - outside of the window loop. So they're only done once, at the start. 
Graphics::Graphics() : m_window(sf::VideoMode({ 800, 600 }), "GEC Start Project"), m_animationManager(m_textureManager), m_zombie(m_animationManager.getAnimation("zombieIdle"))
{
    // Create the SFML window


    // Set up ImGui (the UI library)
    //if (!ImGui::SFML::Init(window))
        //return -1;

    // Zombie Animation Setup
    // Loading the sprite sheets and configuring the animations
         
    m_animationManager.configureAnimation("zombieIdle", "Data/Textures/MaleZombie/idle_combined.png", 15);
    m_animationManager.configureAnimation("zombieWalk", "Data/Textures/MaleZombie/walk_combined.png", 10);
    m_animationManager.configureAnimation("zombieAttack", "Data/Textures/MaleZombie/attack_combined.png", 8);
    m_animationManager.configureAnimation("zombieDeath", "Data/Textures/MaleZombie/dead_combined.png", 12);

    m_zombie.setAnimation(m_animationManager.getAnimation("zombieIdle")); // Plays the idle animation
}

// Will be called in main, running all the graphics/display logic
void Graphics::display()
{
    while (m_window.isOpen())
    {
        // FPS Calculation
        m_frameCount++; // Increments the frame count each loop of the game loop
        // Calculates the FPS every second
        if (m_frameClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            m_fps = static_cast<float>(m_frameCount) / m_frameClock.getElapsedTime().asSeconds();

            m_frameCount = 0;
            m_frameClock.restart();
        }

        update();
        render();
    }
}

// Handles the updating of sprites/textures/animations
void Graphics::update()
{
    m_zombie.update();
}

// The actual logic behind displaying the sprite to the window
void Graphics::render()
{
	m_window.draw(m_zombie);
}