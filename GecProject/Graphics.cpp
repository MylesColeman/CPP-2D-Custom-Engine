#include "Graphics.h"
#include "ExternalHeaders.h"
#include <iostream>

/*
    Use IMGUI for a simple on screen GUI
    See: https://github.com/ocornut/imgui/wiki/
*/
void DefineGUI(Entity* zombie, AnimationManager* animManager, const char* current_item, const char* items[], float fps)
{
    // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("GEC"); // Create a window called "3GP" and append into it.

    ImGui::Text("%.2f FPS", fps); // Displays the FPS to two decimal places

    // Dropdown (combo) box to select animation
    if (ImGui::BeginCombo("Animation", current_item)) // Sets the default value to current_item (which is initially idle)
    {
        // Loops through all possible dropdown items, declared at the top
        for (int i = 0; i < 4; i++)
        {
            bool is_selected = (current_item == items[i]); // Checks whether the current item is already selected
            if (ImGui::Selectable(items[i], is_selected)) // Draws each item in the items array, highlighting the currently selected one
            {
                current_item = items[i]; // Used to display the currently selected item, once closed

                std::string animName = "zombie" + std::string(current_item); // Concatonates the word zombie to the current item, to be used by the animation manager
                zombie->setAnimation(animManager->getAnimation(animName));
            }
            // Scrolls the list to find the currently selected item, making it visible
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}

// Sets up the window, animation manager, and the sprite animator for zombie
Graphics::Graphics() : 
    m_window(sf::VideoMode({ 800, 600 }), "GEC Start Project"), 
    m_animationManager(m_textureManager), 
    m_zombie(m_animationManager.getAnimation("zombieIdle")),
	m_player(m_animationManager.getAnimation("zombieWalk"))
{
    // Set up ImGui (the UI library)
    if (!ImGui::SFML::Init(m_window)) 
        std::cout << "ImGUI could not be loaded!" << std::endl;

	m_inputManager.addListener(&m_player); // Adds the player as a listener to input events

    m_zombie.setAnimation(m_animationManager.getAnimation("zombieIdle")); // Plays the idle animation
	m_player.setAnimation(m_animationManager.getAnimation("zombieWalk"));
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

    m_zombie.update();
	m_inputManager.update(); // Updates the input manager to handle any input events
	m_player.update();

    // Get hitboxes for collision detection
    const CollisionRectangle& playerHitBox = m_player.getHitBox();
    const CollisionRectangle& zombieHitBox = m_zombie.getHitBox();

	// Checks whether the player hitbox intersects with the zombie hitbox
    if (playerHitBox.intersection(zombieHitBox))
        std::cout << "Collision Detected!" << std::endl; // Testing for collision

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
    DefineGUI(&m_zombie, &m_animationManager, m_current_item, m_items, m_fps);

    m_window.draw(m_zombie);
	m_window.draw(m_player);

    // UI needs drawing last
    ImGui::SFML::Render(m_window);

    m_window.display();
}