#include "window.h"

/**
 * @brief Constructor de la clase Window.
 * Crea una ventana SFML con el ancho, alto y titulo especificados.
 * @param width Ancho de la ventana.
 * @param height Alto de la ventana.
 * @param title Titulo de la ventana.
 */
Window::Window(int width, int height, const std::string& title) {
	m_windowPtr = EngineUtilities
		::MakeUnique<sf::RenderWindow>(sf::VideoMode(width, height), title);

	if (!m_windowPtr.isNull()) {
		m_windowPtr->setFramerateLimit(60);
		MESSAGE("Window", "Window", "Window created successfully");
	}
	else {
		ERROR("Window", "Window", "Failed to create window");
	}
}

/**
 * @brief Destructor de la clase Window.
 * Libera el puntero unico a la ventana.
 */
Window::~Window() {
	m_windowPtr.release();
}

/**
 * @brief Maneja eventos de la ventana (como cerrar).
 */
void Window::handleEvents() {
	sf::Event event;
	while (m_windowPtr->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_windowPtr->close();
		}
	}
}

/**
 * @brief Indica si la ventana esta abierta.
 * @return true si la ventana esta abierta, false si no o puntero nulo.
 */
bool Window::isOpen() const {
	if (!m_windowPtr.isNull()) {
		return m_windowPtr->isOpen();
	}
	else {
		ERROR("Window", "isOpen", "Window is null");
		return false;
	}
}

/**
 * @brief Limpia la ventana con el color especificado.
 * @param color Color con que limpiar la ventana (default negro opaco).
 */
void Window::clear(const sf::Color& color) {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->clear(color);
	}
	else {
		ERROR("Window", "clear", "Window is null");
	}
}

/**
 * @brief Dibuja un objeto drawable en la ventana.
 * @param drawable Objeto a dibujar.
 * @param states Estado de renderizado (default).
 */
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->draw(drawable, states);
	}
	else {
		ERROR("Window", "draw", "Window is null");
	}
}

/**
 * @brief Muestra el contenido renderizado en pantalla.
 */
void Window::display() {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->display();
	}
	else {
		ERROR("Window", "display", "Window is null");
	}
}

/**
 * @brief Destruye la ventana liberando recursos.
 */
void Window::destroy() {
	m_windowPtr.release();
}
