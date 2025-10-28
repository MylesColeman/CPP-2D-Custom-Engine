#include "Graphics.h"
#include "ExternalHeaders.h"
#include <iostream>

/*
    Use IMGUI for a simple on screen GUI
    See: https://github.com/ocornut/imgui/wiki/
*/
void DefineGUI(float fps)
{
    // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("GEC"); // Create a window called "GEC"

    ImGui::Text("%.2f FPS", fps); // Displays the FPS to two decimal places

    ImGui::End();
}

// Sets up the window, animation manager, and the sprite animator for zombie
Graphics::Graphics() : 
    m_window(sf::VideoMode({ 800, 600 }), "GEC Start Project"),
	m_textureManager(),
	m_simulation(m_textureManager)
{
    // Set up ImGui (the UI library)
    if (!ImGui::SFML::Init(m_window)) 
        std::cout << "ImGUI could not be loaded!" << std::endl;
}

// Will be called in main, running all the graphics/display logic
void Graphics::display()
{
    while (m_window.isOpen())
    {
        windowEvents();
        update();
        render();
    }

    ImGui::SFML::Shutdown();
}

// Handles the windows events/interactions, movement, editing and closing
void Graphics::windowEvents()
{
    // Process events
    while (const std::optional event = m_window.pollEvent())
    {
        // Feed ImGui
        ImGui::SFML::ProcessEvent(m_window, event.value());

        // User clicked on window close X
        if (event->is<sf::Event::Closed>())
            m_window.close();
    }
}

// Handles the updating of sprites/textures/animations
void Graphics::update()
{
    // ImGui must be updated each frame
    ImGui::SFML::Update(m_window, m_uiDeltaClock.restart());

	m_simulation.update();

    // FPS Calculation
    m_frameCount++; // Increments the frame count each loop of the game loop
    // Calculates the FPS every second
    if (m_frameClock.getElapsedTime().asSeconds() >= 1.0f)
    {
        m_fps = static_cast<float>(m_frameCount) / m_frameClock.getElapsedTime().asSeconds();
        m_frameCount = 0;
        m_frameClock.restart();
    }
}

// The actual logic behind displaying the sprite to the window
void Graphics::render()
{
    // Clear the window
    m_window.clear();

    // The UI gets defined each time
    DefineGUI(m_fps);

    // Loop through a vector of all entities and draw them

    // UI needs drawing last
    ImGui::SFML::Render(m_window);

    m_window.display();
}