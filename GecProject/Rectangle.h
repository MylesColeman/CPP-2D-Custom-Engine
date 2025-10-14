#pragma once

class Rectangle
{
public:
	// Position of the top-left corner
	float m_xPos{ 0.f };
	float m_yPos{ 0.f };

	int m_height{ 0 };
	int m_width{ 0 };

	Rectangle(float xPos = 0.f, float yPos = 0.f, int height = 0, int width = 0) 
		: m_xPos(xPos), m_yPos(yPos), m_height(height), m_width(width) {}

	bool intersection(const Rectangle& other)
	{
		if (this->m_xPos + this->m_width < other.m_xPos || // Checks the right side of this rectangle to the left of the other
			this->m_xPos > other.m_xPos + other.m_width || // Checks the left side of this rectangle to the right of the other
			this->m_yPos + this->m_height < other.m_yPos || // Checks the bottom of this rectangle to the top of the other
			this->m_yPos > other.m_xPos + other.m_height) // Checks the top of this rectangle to the bottom of the other
			return false;
		return true;

	}
};