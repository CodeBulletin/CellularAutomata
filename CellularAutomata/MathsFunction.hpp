#pragma once
#include <SFML/Graphics/Color.hpp>
#include <cmath>

inline float dist00Norm(const float& x, const float& y, const float& maxLength) {
	return (x * x + y * y)/maxLength;
}

sf::Color HSV_to_RGB(float H, float S, float V) {
	H = std::fmod(H, 360.0f);
	float C = V * S;
	float X = (1.0f - std::fabs(std::fmod(H / 60.0f, 2.0f) - 1.0f));
	float M = V - C;

	int n = (int)std::floor(H/60.0f);
	switch (n)
	{
		case 0: return sf::Color((C + M) * 255, (X + M) * 255, (    M) * 255, 255); break;
		case 1: return sf::Color((X + M) * 255, (C + M) * 255, (    M) * 255, 255); break;
		case 2: return sf::Color((    M) * 255, (C + M) * 255, (X + M) * 255, 255); break;
		case 3: return sf::Color((    M) * 255, (X + M) * 255, (C + M) * 255, 255); break;
		case 4: return sf::Color((X + M) * 255, (    M) * 255, (C + M) * 255, 255); break;
		case 5: return sf::Color((C + M) * 255, (    M) * 255, (X + M) * 255, 255); break;
		default: return sf::Color(255, 255, 255, 255);
	}
}

inline sf::Color getColor(const float& angle, const float& len, const float x, const float y, const float maxLength) {
	sf::Color col = HSV_to_RGB(angle + len * dist00Norm(x, y, maxLength), 1.0f, 1.0f);
	return col;
}