#pragma once
#include <string>
#include <SFML/Graphics.hpp>
class CellularAutomata1D {
public:
	CellularAutomata1D() : w(0), h(0), rule(0), wrapArround(false) {}
	CellularAutomata1D(int, int, uint8_t, bool);
	void setRule(uint8_t);
	void Reset();
	void updateImage(int);
	int w, h;
	uint8_t rule;
	bool wrapArround;
	unsigned int ruleset[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	sf::Image image;
	sf::Texture texture;
	sf::Sprite DisplaySprite;
};
