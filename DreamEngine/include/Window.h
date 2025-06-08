#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Gestiona la ventana principal de la aplicación: creación, eventos y renderizado.
 */
class
  Window {
public:
  Window() = default;

  /**
   * @brief Constructor que inicializa ventana con dimensiones y título.
   * @param width  Ancho de la ventana.
   * @param height Alto de la ventana.
   * @param title  Título a mostrar en la barra superior.
   */
  Window(int width, int height, const std::string& title);

  ~Window();

  /**
   * @brief Maneja eventos como entrada de usuario y cierre.
   */
  void
    handleEvents();

  /**
   * @brief Verifica si la ventana sigue abierta.
   * @return true si la ventana está activa.
   */
  bool
    isOpen() const;

  /**
   * @brief Limpia el buffer de render con el color especificado.
   * @param color Color de fondo. Por defecto: negro con opacidad 225.
   */
  void
    clear(const sf::Color& color = sf::Color(0, 0, 0, 225));

  /**
   * @brief Dibuja un objeto gráfico en la ventana.
   * @param drawable Elemento visual (shape, sprite, texto...).
   * @param states   Estados de render. Por defecto: RenderStates::Default.
   */
  void
    draw(const sf::Drawable& drawable,
      const sf::RenderStates& states = sf::RenderStates::Default);

  /**
   * @brief Muestra el contenido renderizado en pantalla.
   */
  void
    display();

  /**
   * @brief Cierra la ventana y libera sus recursos.
   */
  void
    destroy();

private:
  sf::RenderWindow* m_window; ///< Puntero a la ventana SFML.
  sf::View m_view;            ///< Vista activa usada en la ventana.
};
