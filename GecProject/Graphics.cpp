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
	m_gameView(sf::FloatRect({ 0.f, 0.f }, { 320, 180 })), // Sets the game view to a more readable resolution
    m_simulation(m_textureManager),
    m_titleText(m_font),
    m_instructionText(m_font),
    m_scoreText(m_font),
    m_hudScoreText(m_font)
{
	m_window.setPosition({ 0, 0 }); // Sets the window position to the top-left of the screen

    m_window.setVerticalSyncEnabled(true); // Enables VSync to limit FPS

    resizeView(m_window, m_gameView); // Resizes the view to fit the window
    m_window.setView(m_gameView); // Sets the game view to a more readable resolution

    // Set up ImGui (the UI library)
    if (!ImGui::SFML::Init(m_window)) 
        std::cout << "ImGUI could not be loaded!" << std::endl;

    initUI();
    initBg();
}

void Graphics::initBg()
{
	// Sets up the background sprite - done here as it only needs to be done once and doesn't interact with the simulation
    sf::Texture* bgTexture = m_textureManager.getTexture("Data/Textures/World/brickWall_scrollingBg.png");
    
    if (bgTexture)
    {
		bgTexture->setRepeated(true); // Enables texture repeating for scrolling effect

        m_backgroundSprite.emplace(*bgTexture);


        m_backgroundSprite->setTextureRect({ { 0, 0 }, {320, 180} });
    }
}

void Graphics::initUI()
{
    // Loading the font
    if (!m_font.openFromFile("Data/arial.ttf"))
        std::cout << "ERROR: Could not load Data/arial.ttf" << std::endl;

    // Title Text
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(20);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setString("GEC GAME");

    sf::FloatRect titleRect = m_titleText.getLocalBounds();
	m_titleText.setOrigin({ titleRect.size.x / 2.0f, titleRect.size.y / 2.0f }); // Centres the origin
	m_titleText.setPosition({ 160.f, 50.f }); // Positions the title at the top centre of the screen

	// Instruction Text (Press Enter to Start)
    m_instructionText.setFont(m_font);
    m_instructionText.setCharacterSize(10);
    m_instructionText.setFillColor(sf::Color::Yellow);
    m_instructionText.setString("Press Enter to Start");

    sf::FloatRect instrRect = m_instructionText.getLocalBounds();
	m_instructionText.setOrigin({ instrRect.size.x / 2.0f, instrRect.size.y / 2.0f }); // Centres the origin
	m_instructionText.setPosition({ 160.f, 100.f }); // Positions the instruction below the title

	// Score Text
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(12);
    m_scoreText.setFillColor(sf::Color::Green);

    // In-Game HUD
    // Score Text
	m_hudScoreText.setFont(m_font);
    m_hudScoreText.setCharacterSize(10);
    m_hudScoreText.setFillColor(sf::Color::White);
    m_hudScoreText.setPosition({ 10.f, 30.f });

    // Health Bar Background
    m_healthBarBg.setSize({ 100.f, 15.f });
	m_healthBarBg.setFillColor(sf::Color(50, 50, 50)); // Dark grey background
    m_healthBarBg.setPosition({ 10.f, 10.f });

    // Health Bar Foreground
    m_healthBarFg.setSize({ 100.f, 15.f });
	m_healthBarFg.setFillColor(sf::Color::Green); // Green foreground
    m_healthBarFg.setPosition({ 10.f, 10.f });
}

// Draws the HUD elements (Health, score, etc)
void Graphics::drawHUD()
{
    // Safety Check: If player is dead or level not loaded, don't draw HUD
    if (!m_simulation.getPlayer()) return;

	const float maxHealth = static_cast<float>(m_simulation.getPlayer()->getMaxHealth());
    float currentHealth = static_cast<float>(m_simulation.getPlayer()->getHealth());

	// Clamp health to 0 so bar doesn't flip if negative (shouldn't happen normally)
    if (currentHealth < 0.f) currentHealth = 0.f;

	// Calculate width ratio (0.0 to 1.0) how much of the health bar to fill
    float widthRatio = currentHealth / maxHealth;

	// Used to scale the health bar
    float maxWidth = m_healthBarBg.getSize().x;

    // Resize the foreground (Green) bar (the actual health)
    m_healthBarFg.setSize({ maxWidth * widthRatio, m_healthBarBg.getSize().y });

	m_hudScoreText.setString("Score: " + std::to_string(m_simulation.getScore())); // Updates the score text

    // Draws the HUD
    m_window.draw(m_healthBarBg);
    m_window.draw(m_healthBarFg);
    m_window.draw(m_hudScoreText);
}

// Will be called in main, running all the graphics/display logic
void Graphics::display()
{
	m_deltaClock.restart();
    while (m_window.isOpen())
    {
        float deltaTime = m_deltaClock.restart().asSeconds(); // Calculates the delta time for each loop
        windowEvents();

		// Game State Management
        if (m_state == GameState::Frontend) // Frontend State
        {
			// Begins the game on Enter key press
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
				m_simulation.reset(); // Ensures everything is reset for the start of the game
                m_state = GameState::Ingame;
            }
        }
		else if (m_state == GameState::Ingame) // Ingame State
        {
            // Fixed timestep physics update loop
            m_accumulator += deltaTime;
            while (m_accumulator >= m_fixedTimestep)
            {
                m_simulation.update(m_fixedTimestep); // Update the simulation with a fixed timestep
                m_accumulator -= m_fixedTimestep; // Decrease the accumulator by the fixed timestep
            }

			// Check for death condition
            if (m_simulation.isGameOver())
            {
                m_state = GameState::Endgame; // Switch to endgame state

				m_scoreText.setString("Final Score: " + std::to_string(m_simulation.getScore())); // Updates the score text

                sf::FloatRect scoreRect = m_scoreText.getLocalBounds();
                m_scoreText.setOrigin({ scoreRect.size.x / 2.0f, scoreRect.size.y / 2.0f });
                m_scoreText.setPosition({ 160.f, 80.f });
            }
        }
		else if (m_state == GameState::Endgame) // Endgame State
        {
            // Restarts the game on Enter key press
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
				m_simulation.reset(); // Ensures everything is reset for the start of the game
                m_state = GameState::Ingame;
            }
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

		// Window resized - adjust the view accordingly
        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            resizeView(m_window, m_gameView);
            m_window.setView(m_gameView);
        }
    }
}

// Resizes the view when the window is resized
void Graphics::resizeView(const sf::Window& window, sf::View& view)
{
	float windowRatio = window.getSize().x / (float)window.getSize().y; // Aspect ratio of the window
	float viewRatio = view.getSize().x / (float)view.getSize().y; // Aspect ratio of the view
    float xSize = 1.f;
	float ySize = 1.f;
    float xPos = 0.f;
    float yPos = 0.f;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio) // Window is taller than the view
		horizontalSpacing = false;

	// Adjust the viewport - adding black bars where necessary
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

	view.setViewport(sf::FloatRect({ xPos, yPos }, { xSize, ySize })); // Sets the new viewport
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
		m_frameCount = 0; // Resets the frame count
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

	float alpha = m_accumulator / m_fixedTimestep; // Calculates the alpha for interpolation

	// Only renders if in the ingame state or the Endgame state
    if (m_state == GameState::Ingame || m_state == GameState::Endgame)
    {
        sf::Vector2f camPos = m_gameView.getCenter();

		// Background Scrolling Logic - 0.5 speed
        int xOffset = static_cast<int>(camPos.x * 0.5f);
        int yOffset = static_cast<int>(camPos.y * 0.5f);

        if (m_backgroundSprite.has_value())
        {
            m_backgroundSprite->setTextureRect({ {xOffset, yOffset}, {320, 180} }); // Updates the texture rect for scrolling effect
            m_backgroundSprite->setPosition({ camPos.x - 160, camPos.y - 90 }); // Centres the background on the camera

            // Sets the view to the game view for rendering the game world
            m_window.setView(m_gameView);
            m_window.draw(*m_backgroundSprite);
        }
        
        // Logic behind camera movement which follows the player
        if (m_simulation.getPlayer())
        {
			// Interpolating the player's position for smooth camera movement
            sf::Vector2f currentPos = m_simulation.getPlayer()->getPosition();
            sf::Vector2f prevPos = m_simulation.getPlayer()->getPreviousPosition();
            sf::Vector2f lerpPos = prevPos + (currentPos - prevPos) * alpha;

            sf::Vector2f levelSize = m_simulation.getLevelSize();
            float levelWidth = levelSize.x;
            float levelHeight = levelSize.y;

			// Half dimensions of the view
            float halfViewW = m_gameView.getSize().x / 2.f;
            float halfViewH = m_gameView.getSize().y / 2.f;

            // Defining the camera bounds
            // X
			if (levelWidth <= m_gameView.getSize().x) // Level smaller than view width
                lerpPos.x = halfViewW;
            else
            {
                float minX = halfViewW;
                float maxX = levelWidth - minX;

                // Clamping the camera to the level bounds
                if (lerpPos.x < minX) lerpPos.x = minX;
                else if (lerpPos.x > maxX) lerpPos.x = maxX;
            }
            

            // Y
			if (levelHeight <= m_gameView.getSize().y) // Level smaller than view height
                lerpPos.y = halfViewH;
            else
            {
                float minY = halfViewH;
                float maxY = levelHeight - minY;

				// Clamping the camera to the level bounds
                if (lerpPos.y < minY) lerpPos.y = minY;
                else if (lerpPos.y > maxY) lerpPos.y = maxY;
            }

            // Applying the new clamped camera position
            m_gameView.setCenter(lerpPos);
        }

        m_window.setView(m_gameView); // Updates the view

		// Lambda function to draw entities with interpolation
        auto drawInterpolated = [&](Entity* entity, sf::BlendMode blendMode = sf::BlendAlpha)
            { 
				// Gets actual and previous positions
                sf::Vector2f actualPos = entity->getPosition();
                sf::Vector2f oldPos = entity->getPreviousPosition();

				sf::Vector2f visualPos = oldPos + (actualPos - oldPos) * alpha; // Interpolated position

				entity->setPosition(visualPos); // Move to visual spot for drawing
				m_window.draw(*entity, blendMode); // Draws the entity at the interpolated position
				entity->setPosition(actualPos); // Returns the entity to its actual position for physics calculations (not visual)
            };

        // Loops through the vector of entities created in the simulation, and draws them
        for (const auto& entity : m_simulation.getEntities())
			drawInterpolated(entity.get());

        // Loops through the bullets and draws them
        for (const auto& bullet : m_simulation.getBullets())
        {
            // Only draws the bullet if its active
            if (bullet->isActive())
                drawInterpolated(bullet.get(), sf::BlendAdd);
        }

        // Debugging hitbox visualisers
        for (auto& pair : m_simulation.m_triggerColliders)
            m_window.draw(m_simulation.m_triggerHitboxVisualiser);
    }

	// UI View
	sf::View uiView(sf::FloatRect({ 0.f, 0.f }, { 320.f, 180.f })); // Sets the UI view to a fixed resolution
    m_window.setView(uiView);

	// Frontend & Endgame Screens
	if (m_state == GameState::Frontend) // Frontend State
    {
        m_titleText.setString("GEC GAME");
        m_titleText.setFillColor(sf::Color::White);
        m_instructionText.setString("Press Enter to Start");

        m_window.draw(m_titleText);
        m_window.draw(m_instructionText);
    }
    else if (m_state == GameState::Ingame)
        drawHUD();
	else if (m_state == GameState::Endgame) // Endgame State
    {
        m_titleText.setString("GAME OVER");
        m_titleText.setFillColor(sf::Color::Red);
        m_instructionText.setString("Press Enter to Restart");

        m_window.draw(m_titleText);
        m_window.draw(m_scoreText);
        m_window.draw(m_instructionText);
    }

    // UI needs drawing last
    ImGui::SFML::Render(m_window);

    m_window.display();
}