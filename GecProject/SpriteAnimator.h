#pragma once
#include "AnimationManager.h"
#include <SFML/Graphics.hpp>

class SpriteAnimator : public sf::Sprite
{
public:
    // Initialize the sprite with the texture of the animation, as sprite doesn't have a default constructor
    explicit SpriteAnimator(const Animation& animation) : sf::Sprite(*animation.texture) {};

    // Sets the animation to be used, starting it on the first frame
    void setAnimation(const Animation& animation)
    {
        m_animation = &animation;
        this->setTexture(*m_animation->texture, true);
        m_currentFrame = 0;
        m_animClock.restart();

        // Calculates and sets the intRect for the first frame. Otherwise the entire spritesheet would be shown till it was set in update
        int intRectsYPos = m_currentFrame * m_animation->spriteHeight;
        this->setTextureRect(sf::IntRect({ 0, intRectsYPos }, { m_animation->spriteWidth, m_animation->spriteHeight }));
    }

    // Checks whether there is a assigned animation, and if so updates it based on a set time
    void update()
    {
        // Checks whether there is an assigned animation
        if (m_animation == nullptr)
            return;

        // After the set time, updates to the next sprite
        if (m_animClock.getElapsedTime().asSeconds() > m_animation->timeBetweenFrames)
        {
            m_currentFrame++;

            // If reached the last frame, loops back around
            if (m_currentFrame >= m_animation->numFrames)
            {
                m_currentFrame = 0;
            }

            int intRectsYPos = m_currentFrame * m_animation->spriteHeight; // Sets the intRect's Y position based on the current frame 

            /* Updates the intRect, as all sprite sheets are composed vertically 'x' can remain as 0 but 'y' is updated based on the calculation above.The size
             is calculated using the spriteWidth and spriteHeight variables from the Animation struct */
            this->setTextureRect(sf::IntRect({ 0, intRectsYPos }, { m_animation->spriteWidth, m_animation->spriteHeight }));

            m_animClock.restart();
        }
    }
private:
    sf::Clock m_animClock;
    const Animation* m_animation{ nullptr };
    int m_currentFrame{ 0 };
};