#pragma once
#include "Prerequisites.h"
#include "Window.h"

/**
 * @class BaseApp
 * @brief Clase principal encargada del ciclo de vida de la aplicación.
 */
class
  BaseApp {
public:
  BaseApp() = default;

  ~BaseApp();

  /**
   * @brief Ejecuta el ciclo principal de la aplicación.
   * @return Código de salida del programa (0 si fue exitoso).
   */
  int
    run();

  /**
   * @brief Inicializa los recursos de la aplicación.
   * @return true si la inicialización fue exitosa.
   */
  bool
    init();

  /**
   * @brief Actualiza la lógica de la aplicación por frame.
   */
  void
    update();

  /**
   * @brief Renderiza los elementos visuales.
   */
  void
    render();

  /**
   * @brief Libera los recursos utilizados por la aplicación.
   */
  void
    destroy();

private:
  Window* m_window;           ///< Puntero a la ventana principal.
  sf::CircleShape* m_circle;  ///< Figura a renderizar.
};
