/*
    GecProject - For GEC students to use as a start point for their projects.
    Already has SFML linked and ImGui set up.
*/

#include "ExternalHeaders.h"
#include "RedirectCout.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "SpriteAnimator.h"
#include <iostream>
#include <SFML/Graphics.hpp>

void DefineGUI(SpriteAnimator* zombie, AnimationManager* animManager);

// Used by ImGUI dropdown (combo) box
static const char* items[] = { "Idle", "Walk", "Attack", "Death" };
static const char* current_item = "Idle";
int main()
{
    // Redirect cout to the Visual Studio output pane
    outbuf ob;
    std::streambuf* sb{ std::cout.rdbuf(&ob) };

    // Redirect cerr
    outbuferr oberr;
    std::streambuf* sberr{ std::cerr.rdbuf(&oberr) };

    // Turn on memory leak checking
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Clock required by ImGui
    sf::Clock uiDeltaClock;
  
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Feed ImGui
            ImGui::SFML::ProcessEvent(window, event.value());

            // User clicked on window close X
            if (event->is<sf::Event::Closed>())
                window.close();                          
        }

        // ImGui must be updated each frame
        ImGui::SFML::Update(window, uiDeltaClock.restart());

        zombie.update();

        // The UI gets defined each time
        DefineGUI(&zombie, &animationManager);

        // Clear the window
        window.clear();
       
		window.draw(zombie);

        // UI needs drawing last
        ImGui::SFML::Render(window);

        window.display();
    }

    std::cout << "Finished!" << std::endl;

	ImGui::SFML::Shutdown();

    return 0;
}

/* 
    Use IMGUI for a simple on screen GUI
    See: https://github.com/ocornut/imgui/wiki/
*/
void DefineGUI(SpriteAnimator* zombie, AnimationManager* animManager)
{
    // Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("GEC"); // Create a window called "3GP" and append into it.

    ImGui::Text("%.2f FPS", fps); // Displays the FPS to two decimal places

	// Dropdown (combo) box to select animation
	if (ImGui::BeginCombo("Animation", current_item)) // Sets the default value to current_item (which is initially idle)
    {
        // Loops through all possible dropdown items, declared at the top
        for (int i = 0; i < IM_ARRAYSIZE(items); i++)
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