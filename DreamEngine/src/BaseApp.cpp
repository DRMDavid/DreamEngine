// Estilo de codificación:
// - camelCase para variables y métodos
// - PascalCase para clases
// - Tabulación: 2 espacios
// - Líneas de 80 a 90 caracteres máximo
// - Prefijo g_ para variables globales
// - Las clases y estructuras se definen con la llave de apertura en la misma línea
// - Los métodos dentro de clases se declaran con salto de línea entre el tipo y el nombre
// - La apertura del cuerpo del método se realiza en una nueva línea

#include "BaseApp.h"

// Destructor implementation
BaseApp::~BaseApp()
{
}


int
BaseApp::run()
{
	if (!init()) {
		ERROR("BaseApp",
			"run",
			"Initializes result on a false statement, check method validations");
	}
	while (m_window->isOpen()) {
		m_window->handleEvents();
		update();
		render();
	}
	Destroy();
	return 0;
}

// Initialization, update, render, destroy, and event handling methods
bool
BaseApp::init()
{
	m_window = new Window(800, 600, "DreamEngine");
	//m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "DreamEngine");
	m_circle = new sf::CircleShape(100.0f); // radio 100
	m_circle->setFillColor(sf::Color::Magenta);
	m_circle->setPosition(200.f, 150.f);
	return true;
}
void
BaseApp::update()
{
	// Update the application state
}
void
BaseApp::render()
{
	m_window->clear();
	m_window->draw(*m_circle);
	m_window->display();
}
void
BaseApp::Destroy()
{
	delete m_circle;
	m_window->destroy();
}


