#pragma once
#include "Prerequisites.h"

class EngineGUI;

class Window {
public:
	Window() = default;
	Window(int width, int height, const std::string& title);
	~Window();

	void handleEvents(EngineGUI& engineGUI);
	bool isOpen() const;
	void clear(const sf::Color& color = sf::Color(0, 0, 0, 255));
	void draw(const sf::Drawable& drawable,
		const sf::RenderStates& states = sf::RenderStates::Default);
	void display();
	void update();
	void render();
	void destroy();

private:
	sf::View m_view;

public:
	sf::Time deltaTime;
	sf::Clock m_clock;
	EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr;
};
