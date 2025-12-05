#include "Graphics.h"
#include "ExternalHeaders.h"

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

// Sets up the window and the simulation
Graphics::Graphics() :
    m_window(sf::VideoMode::getDesktopMode(), "GEC Start Project", sf::Style::Default),
    m_gameView(sf::FloatRect({ 0.f, 0.f }, { 320, 180 })),
	m_simulation(m_textureManager)
{
    m_window.setPosition({ 0, 0 });

    m_window.setVerticalSyncEnabled(true); // Enables VSync to limit FPS

    resizeView(m_window, m_gameView);
    m_window.setView(m_gameView); // Sets the game view to a more readable resolution

    // Set up ImGui (the UI library)
    if (!ImGui::SFML::Init(m_window)) 
        std::cout << "ImGUI could not be loaded!" << std::endl;
}

// Will be called in main, running all the graphics/display logic
void Graphics::display()
{
	m_deltaClock.restart();
    while (m_window.isOpen())
    {
        float deltaTime = m_deltaClock.restart().asSeconds(); // Calculates the delta time for each loop
        windowEvents();

        m_accumulator += deltaTime;
        while (m_accumulator >= m_fixedTimestep)
        {
            m_simulation.update(m_fixedTimestep);
            m_accumulator -= m_fixedTimestep;
        }

        update(deltaTime);
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

        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            resizeView(m_window, m_gameView);
            m_window.setView(m_gameView);
        }
    }
}

void Graphics::resizeView(const sf::Window& window, sf::View& view)
{
    float windowRatio = window.getSize().x / (float)window.getSize().y;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
    float xSize = 1.f;
	float ySize = 1.f;
    float xPos = 0.f;
    float yPos = 0.f;

	bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
		horizontalSpacing = false;

    if (horizontalSpacing)
    {
		xSize = viewRatio / windowRatio;
        xPos = (1.f - xSize) / 2.f;
    }
    else
    {
        ySize = windowRatio / viewRatio;
		yPos = (1.f - ySize) / 2.f;
    }

    view.setViewport(sf::FloatRect({ xPos, yPos }, { xSize, ySize }));
}

// Handles the graphical updating of the simulation, which in turn handles the graphical updating of entities (e.g. animations & movement). Also updates the ImGui and the FPS counter
void Graphics::update(float deltaTime)
{
    // ImGui must be updated each frame
    ImGui::SFML::Update(m_window, m_uiDeltaClock.restart());

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
    m_window.clear(sf::Color(139, 142, 135));

    // The UI gets defined each time
    DefineGUI(m_fps);

    if (m_simulation.getPlayer())
    {
        sf::Vector2f playerPos = m_simulation.getPlayer()->getPosition();
        m_gameView.setCenter(playerPos);

        if (playerPos.x - (m_gameView.getSize().x / 2) < 0)
        {
            std::cout << "Left bound reached" << std::endl;
        }
        else if (playerPos.x + (m_gameView.getSize().x / 2) > m_window.getSize().x)
        {
            std::cout << "Right bound reached" << std::endl;
        }

        if (playerPos.y - (m_gameView.getSize().y / 2) < 0)
        {
            std::cout << "Top bound reached" << std::endl;
        }
        else if (playerPos.y + (m_gameView.getSize().y / 2) > m_window.getSize().y)
        {
            std::cout << "Bottom bound reached" << std::endl;
        }
    }

	m_window.setView(m_gameView); // Updates the view

    // Loops through the vector of entities created in the simulation, and draws them
    for (const auto& entity : m_simulation.getEntities())
        m_window.draw(*entity);

	// Debugging hitbox visualisers
    for (auto& pair : m_simulation.m_triggerColliders)
        m_window.draw(m_simulation.m_triggerHitboxVisualiser);

    // UI needs drawing last
    ImGui::SFML::Render(m_window);

    m_window.display();
}