#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Clase que encapsula una ventana SFML para renderizado y gestion de eventos.
 *
 * Proporciona interfaz para crear, manejar eventos, limpiar, dibujar, mostrar y destruir la ventana.
 */
class Window {
public:
  /// Constructor por defecto
  Window() = default;

  /**
   * @brief Constructor que crea una ventana con dimensiones y titulo especificos.
   * @param width Ancho de la ventana en pixeles.
   * @param height Alto de la ventana en pixeles.
   * @param title Titulo de la ventana.
   */
  Window(int width, int height, const std::string& title);

  /// Destructor que libera recursos
  ~Window();

  /// Procesa eventos de la ventana (como cierre, teclado, mouse)
  void handleEvents();

  /// Indica si la ventana sigue abierta
  /// @return true si la ventana esta abierta
  bool isOpen() const;

  /**
   * @brief Limpia la ventana con un color especifico (por defecto negro opaco).
   * @param color Color para limpiar la ventana.
   */
  void clear(const sf::Color& color = sf::Color(0, 0, 0, 255));

  /**
   * @brief Dibuja un objeto drawable en la ventana.
   * @param drawable Referencia al objeto drawable SFML.
   * @param states Estados de renderizado (opcional).
   */
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default);

  /// Muestra el contenido dibujado en la ventana
  void display();

  /// Destruye la ventana y libera recursos asociados
  void destroy();

private:
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr; ///< Puntero unico a la ventana SFML
  sf::View m_view; ///< Vista (camara) de la ventana
};
