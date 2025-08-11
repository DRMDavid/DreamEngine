/**
 * @file Window.cpp
 * @brief Implementación de la clase Window, que administra la ventana principal del motor.
 * @details Esta clase encapsula la creación, manejo de eventos, y control del ciclo
 *          de vida de una ventana usando SFML, además de facilitar la integración
 *          con la interfaz gráfica (EngineGUI).
 */

#include "window.h"
#include "EngineGUI.h"

 /**
	* @brief Constructor de la ventana principal.
	* @param width Ancho de la ventana en píxeles.
	* @param height Alto de la ventana en píxeles.
	* @param title Título que se mostrará en la barra de la ventana.
	* @details Crea una ventana SFML con el tamaño y título especificados.
	*          Se establece un límite de FPS de 60.
	* @post Si la ventana no se pudo crear, se genera un mensaje de error.
	*/
Window::Window(int width, int height, const std::string& title) {

	m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
		sf::VideoMode({ static_cast<unsigned int>(width),
										static_cast<unsigned int>(height) }),
		title,
		sf::Style::Default
	);

	if (!m_windowPtr.isNull()) {
		m_windowPtr->setFramerateLimit(60);
		MESSAGE("Window", "Window", "Window created successfully");
	}
	else {
		ERROR("Window", "Window", "Failed to create window");
	}
}

/**
 * @brief Destructor de la ventana.
 * @details Libera el puntero a la ventana.
 */
Window::~Window() {
	m_windowPtr.release();
}

/**
 * @brief Procesa todos los eventos de la ventana.
 * @param engineGUI Referencia al sistema de GUI para delegar eventos.
 * @details Recorre todos los eventos de la ventana y:
 *  - Los pasa a EngineGUI para su procesamiento.
 *  - Cierra la ventana si se recibe el evento de cierre.
 */
void
Window::handleEvents(EngineGUI& engineGUI) {

	while (const std::optional event = m_windowPtr->pollEvent())
	{
		engineGUI.processEvent(*m_windowPtr, *event);

		// Cierra la ventana si se recibe el evento de cerrado
		if (event->is<sf::Event::Closed>())
			m_windowPtr->close();
	}
}

/**
 * @brief Verifica si la ventana está abierta.
 * @return true si la ventana está abierta, false en caso contrario.
 */
bool
Window::isOpen() const {
	if (!m_windowPtr.isNull()) {
		return m_windowPtr->isOpen();
	}
	else {
		ERROR("Window", "isOpen", "Window is null");
		return false;
	}
}

/**
 * @brief Limpia el contenido de la ventana con un color específico.
 * @param color Color con el que se limpiará la ventana.
 */
void
Window::clear(const sf::Color& color) {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->clear(color);
	}
	else {
		ERROR("Window", "clear", "Window is null");
	}
}

/**
 * @brief Dibuja un objeto en la ventana.
 * @param drawable Objeto que implementa sf::Drawable.
 * @param states Estados de renderizado opcionales.
 */
void
Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->draw(drawable, states);
	}
	else {
		ERROR("Window", "draw", "Window is null");
	}
}

/**
 * @brief Muestra en pantalla todo lo que se ha dibujado en el frame actual.
 */
void
Window::display() {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->display();
	}
	else {
		ERROR("Window", "display", "Window is null");
	}
}

/**
 * @brief Actualiza el tiempo delta para el ciclo de juego.
 * @details Reinicia el reloj interno y almacena el tiempo transcurrido
 *          desde la última actualización en `deltaTime`.
 */
void
Window::update() {
	deltaTime = m_clock.restart();
}

/**
 * @brief Renderiza el contenido de la ventana.
 * @details Este método está actualmente vacío y puede ser sobrescrito
 *          o extendido para dibujar elementos propios de la ventana.
 */
void
Window::render() {
}

/**
 * @brief Libera los recursos asociados a la ventana.
 */
void
Window::destroy() {
	m_windowPtr.release();
}
