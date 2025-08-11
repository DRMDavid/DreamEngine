/**
 * @file
 * @brief Clase para la gestión de la ventana principal del motor/juego.
 * @details Encapsula una ventana de SFML y proporciona métodos para manejar
 *  eventos, dibujar elementos, limpiar, actualizar y renderizar la escena.
 */

#pragma once
#include "Prerequisites.h"

class EngineGUI;

/**
 * @class Window
 * @brief Encapsula la funcionalidad de una ventana SFML.
 * @details Gestiona el ciclo de vida de la ventana, eventos, renderizado y
 *  control del tiempo delta (`deltaTime`).
 */
class Window {
public:
  /** @brief Constructor por defecto. */
  Window() = default;

  /**
   * @brief Constructor que crea una ventana con dimensiones y título.
   * @param width Ancho de la ventana en píxeles.
   * @param height Alto de la ventana en píxeles.
   * @param title Título de la ventana.
   */
  Window(int width, int height, const std::string& title);

  /** @brief Destructor que libera los recursos de la ventana. */
  ~Window();

  /**
   * @brief Maneja los eventos de la ventana.
   * @param engineGUI Referencia a la interfaz gráfica para procesar eventos.
   */
  void handleEvents(EngineGUI& engineGUI);

  /**
   * @brief Comprueba si la ventana está abierta.
   * @return true si la ventana sigue abierta.
   */
  bool isOpen() const;

  /**
   * @brief Limpia la ventana con un color dado.
   * @param color Color de limpieza (por defecto negro opaco).
   */
  void clear(const sf::Color& color = sf::Color(0, 0, 0, 255));

  /**
   * @brief Dibuja un objeto en la ventana.
   * @param drawable Referencia al objeto dibujable.
   * @param states Estados de renderizado (opcional).
   */
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default);

  /** @brief Muestra en pantalla el contenido dibujado en el frame actual. */
  void display();

  /** @brief Actualiza el estado de la ventana (tiempo delta, lógica, etc.). */
  void update();

  /** @brief Renderiza el contenido de la ventana (ciclo completo de dibujo). */
  void render();

  /** @brief Libera los recursos asociados a la ventana. */
  void destroy();

private:
  sf::View m_view; ///< Vista utilizada para el renderizado.

public:
  sf::Time deltaTime; ///< Tiempo delta entre frames.
  sf::Clock m_clock; ///< Reloj para medir el tiempo entre frames.
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr; ///< Puntero único a la ventana SFML.
};
