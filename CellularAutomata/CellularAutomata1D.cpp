#include "CellularAutomata1D.hpp"
#include "MathsFunction.hpp"

CellularAutomata1D::CellularAutomata1D(int _w, int _h, uint8_t _rule, bool _wa) : w(_w), h(_h),
rule(_rule), wrapArround(_wa) {
	setRule(rule);
	Reset();
	texture.create(w, h);
	DisplaySprite.setTexture(texture);
}

void CellularAutomata1D::setRule(uint8_t _rule) {
	rule = _rule;
	for (int i = 0; i < 8; i++) {
		uint8_t x = rule << i;
		ruleset[7-i] = x >> 7;
	}
}

void CellularAutomata1D::Reset() {
	image.create(w, h, sf::Color::Black);
	float minVal = std::min(w, h);
	float xScale = w / minVal;
	float yScale = h / minVal;
	float max_length = dist00Norm(w / xScale, h / yScale, 1);
	sf::Color col = getColor((rule / 255.0f) * 360.0f, 40, w / (2.0f * xScale), 0, max_length);
	image.setPixel(w / 2, 0, col);
}

void CellularAutomata1D::updateImage(int row) {
	texture.update(image);
	float minVal = std::min(w, h);
	float xScale = w / minVal;
	float yScale = h / minVal;
	float max_length = dist00Norm(w / xScale, h / yScale, 1);
	if (!wrapArround) {
		for (int i = 1; i < w - 1; i++) {
			auto px1 = image.getPixel(i - 1, row);
			auto px2 = image.getPixel(i    , row);
			auto px3 = image.getPixel(i + 1, row);
			unsigned int left   = (px1.r > 0) || (px1.g > 0) || (px1.b > 0);
			unsigned int Middle = (px2.r > 0) || (px2.g > 0) || (px2.b > 0);
			unsigned int Right  = (px3.r > 0) || (px3.g > 0) || (px3.b > 0);
			uint8_t val = ruleset[left * 4 + Middle * 2 + Right];
			sf::Color col = getColor((rule / 255.0f) * 360.0f, 40, i / xScale, (row + 1) / yScale, max_length);
			col.r *= val;
			col.g *= val;
			col.b *= val;
			image.setPixel(i, row + 1, col);
		}
	}
	else {
		for (int x = 0; x < w; x++) {
			int i0 = x == 0 ? w - 1 : x - 1;
			int i1 = x;
			int i2 = x == w - 1 ? 0 : x + 1;
			auto px1 = image.getPixel(i0, row);
			auto px2 = image.getPixel(i1, row);
			auto px3 = image.getPixel(i2, row);
			unsigned int left = (px1.r > 0) || (px1.g > 0) || (px1.b > 0);
			unsigned int Middle = (px2.r > 0) || (px2.g > 0) || (px2.b > 0);
			unsigned int Right = (px3.r > 0) || (px3.g > 0) || (px3.b > 0);
			uint8_t val = ruleset[left * 4 + Middle * 2 + Right];
			sf::Color col = getColor((rule / 255.0f) * 360.0f, 40, x / xScale, (row + 1) / yScale, max_length);
			col.r *= val;
			col.g *= val;
			col.b *= val;
			image.setPixel(i1, (row + 1) % h, col);
		}
	}
}
