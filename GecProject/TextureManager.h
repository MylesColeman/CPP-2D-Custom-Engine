#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class TextureManager
{
public:
	// Creates a method that checks whether the texture has already been loaded, if so it doesn't load it again. If not it attemps to load it.
	sf::Texture* getTexture(const std::string& filePath)
	{
		// A check whether the texture hasn't been loaded
		if (m_textures.find(filePath) == m_textures.end())
		{
			sf::Texture texture;
			if (!texture.loadFromFile(filePath))
			{
				std::cout << "Texture " << filePath << " could not be loaded!" << std::endl;
				return nullptr;
			}
			else
				m_textures[filePath] = texture;
		}

		return &m_textures.at(filePath); // Returns the texture, either if already loaded, or just loaded
	}
private:
	std::unordered_map<std::string, sf::Texture> m_textures;
};