#include "CellularAutomata1D.hpp"
#include <sstream>

static const std::string name = "Cellular Automata";
static int width;
static int height;

static int ImageWidth = 192;
static int ImageHeight = 108;

static int FrameRate = -1;


int index = 0;

int val = 0;

bool takeinput = true;
bool showRule = false;
bool loopMode = false;
bool loopRule = false;
bool vSync = true;
std::string Rule = "";

void loop(sf::RenderWindow&, CellularAutomata1D&);
void eventManager(sf::Event&, sf::RenderWindow&, CellularAutomata1D&);

uint8_t StringToInt(std::string& str) {
	if (str.size() == 0) str = '0';
	std::stringstream ss(str);
	int x;
	ss >> x;
	return x;
}

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	auto vid = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window;
	window.create(vid, name, sf::Style::Fullscreen, settings);

	width = vid.width;
	height = vid.height;

	sf::Event event;
	if(vSync) window.setVerticalSyncEnabled(true);
	if (FrameRate > 0) window.setFramerateLimit(FrameRate);

	CellularAutomata1D cellular_automata(ImageWidth, ImageHeight, 0, loopRule);
	cellular_automata.setRule(StringToInt(Rule));
	cellular_automata.DisplaySprite.scale({ (float)width / (float)ImageWidth, (float)height / (float)ImageHeight });
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setPosition(0.0f, 0.0f);

	sf::Text text;
	sf::Font font;
	font.loadFromFile("data/font.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::Blue);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			eventManager(event, window, cellular_automata);
		}
		loop(window, cellular_automata);
		if (showRule || takeinput) {
			if (takeinput) {
				text.setCharacterSize(128);
				text.setString("Set Rule: " + Rule);
				text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f,
					text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
				text.setPosition(width/2, height/2);
			}
			else {
				text.setCharacterSize(32);
				text.setString("Rule: " + Rule);
				rs.setSize({ text.getLocalBounds().width + 32.0f , text.getLocalBounds().height + 32.0f });
				text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);
				text.setPosition(16, 16);
				window.draw(rs);
			}
			window.draw(text);
		}
		window.display();
	}

	return 0;
}

void eventManager(sf::Event& event, sf::RenderWindow& win, CellularAutomata1D& cellular_automata) {
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (takeinput) {
			if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
				if (Rule.size() < 3) Rule += (char)(event.key.code + 22);
			}
			if (event.key.code == sf::Keyboard::BackSpace) {
				if (Rule.size() > 0) Rule.pop_back();
			}
		}
		if (event.key.code == sf::Keyboard::Escape) {
			win.close();
			exit(0);
		}
		else if (event.key.code == sf::Keyboard::Tilde && !takeinput) {
			takeinput = true;
		}
		else if (event.key.code == sf::Keyboard::Enter && takeinput) {
			takeinput = false;
			int n = StringToInt(Rule);
			cellular_automata.setRule(n);
			cellular_automata.Reset();
			index = 0;
		}
		else if (event.key.code == sf::Keyboard::Space && !takeinput) {
			cellular_automata.Reset();
			index = 0;
		}
		else if (event.key.code == sf::Keyboard::Tab) {
			showRule = !showRule;
		}
		if (event.key.code == sf::Keyboard::Equal) {
			cellular_automata.Reset();
			cellular_automata.wrapArround = !cellular_automata.wrapArround;
			index = 0;
		}
		if (event.key.code == sf::Keyboard::L) {
			loopMode = !loopMode;
			index = 0;
		}
		break;
	default:
		break;
	}
}

void loop(sf::RenderWindow& win, CellularAutomata1D& cellular_automata) {
	win.clear(sf::Color::Black);
	if (!takeinput) {
		win.draw(cellular_automata.DisplaySprite);
		if (!cellular_automata.wrapArround) {
			cellular_automata.updateImage(index < ImageHeight - 1 ? index : ImageHeight - 2);
		}
		else {
			cellular_automata.updateImage(index % ImageHeight);
		}
		index++;
		if (index == ImageHeight && loopMode && !takeinput) {
			index = 0;
			val = (val + 1) % 256;
			Rule = std::to_string(val);
			cellular_automata.Reset();
			cellular_automata.setRule(val);
		}
	}
}