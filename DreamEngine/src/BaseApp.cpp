#include <BaseApp.h>

BaseApp::~BaseApp()
{
}

int 
BaseApp::run() {
	if (!init() {
		ERROR ("BaseApp",
			  "run",
			  "Initializes result on a false statemente, check method validations");
	}
	while (m_window->isOpen()) {
		handleEvents();
		update();
		render();
	}
	destroy();
    
	return 0;
}

bool 
BaseApp::init() {
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "DreamEngine");
	m_circle = new sf::CircleShape(100.0f); // radio 100
	m_circle->setFillColor(sf::Color::Magenta);
	m_circle->setPosition(200.f, 150.f);

	return true;
}

void
BaseApp::update() {
}

void
BaseApp::render() {

	m_window->clear();

	m_window->draw(* m_circle);

	m_window->display();

}

void 
BaseApp::destroy() {

	delete m_window;

	delete m_circle;

}

void 
BaseApp::handleEvents() {

	sf::Event event;
	while (m_window->pollEvent(event)) {
		// Cerrar la ventana si el usuario lo indica
		if (event.type == sf::Event::Closed)
			m_window->close();

	}
}


