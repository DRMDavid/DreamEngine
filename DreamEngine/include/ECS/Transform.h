/**
 * @file
 * @brief Componente de transformación para posición, rotación y escala.
 * @details Proporciona utilidades para manipular la posición en el mundo,
 *  aplicar rotaciones y escalas, así como métodos de movimiento como `seek`.
 *  Hereda de `Component` y se clasifica como `TRANSFORM`.
 */

#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "Prerequisites.h"
#include "Component.h"
#include "Window.h"

class Window;

/**
 * @class Transform
 * @brief Componente que almacena y manipula posición, rotación y escala.
 * @details Permite operaciones básicas de movimiento y posicionamiento,
 *  incluyendo el método `seek` para desplazarse hacia un objetivo.
 */
class Transform : public Component {
public:
  /**
   * @brief Constructor por defecto.
   * @details Inicializa la posición en (0,0), la rotación en (0,0) y la
   *  escala en (1,1). Establece el tipo como `TRANSFORM`.
   */
  Transform() :
    m_position(0.f, 0.f),
    m_rotation(0.f, 0.f),
    m_scale(1.f, 1.f),
    Component(ComponentType::TRANSFORM) {
  }

  /** @brief Destructor virtual por defecto. */
  virtual ~Transform() = default;

  /** @brief Inicializa el componente (sin implementación específica). */
  void start() override {}

  /**
   * @brief Actualiza el componente (sin implementación específica).
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float deltaTime) override {}

  /**
   * @brief Renderiza el componente (sin implementación específica).
   * @param window Puntero compartido a la ventana de renderizado.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override {}

  /** @brief Libera recursos del componente (sin implementación específica). */
  void destroy() {}

  /**
   * @brief Desplaza la posición hacia un objetivo.
   * @param targetPosition Posición objetivo.
   * @param speed Velocidad de movimiento (unidades por segundo).
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   * @param range Distancia mínima para detenerse.
   */
  void seek(const sf::Vector2f& targetPosition,
    float speed,
    float deltaTime,
    float range) {
    sf::Vector2f direction = targetPosition - m_position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > range) {
      direction.x /= length;
      direction.y /= length;
      m_position += direction * speed * deltaTime;
    }
  }

  /**
   * @brief Establece la posición.
   * @param _position Nueva posición.
   */
  void setPosition(const sf::Vector2f& _position) { m_position = _position; }

  /**
   * @brief Establece la rotación.
   * @param _rotation Nueva rotación.
   */
  void setRotation(const sf::Vector2f& _rotation) { m_rotation = _rotation; }

  /**
   * @brief Establece la escala.
   * @param _scale Nueva escala.
   */
  void setScale(const sf::Vector2f& _scale) { m_scale = _scale; }

  /**
   * @brief Obtiene la posición.
   * @return Referencia a la posición.
   */
  sf::Vector2f& getPosition() { return m_position; }

  /**
   * @brief Obtiene la rotación.
   * @return Referencia a la rotación.
   */
  sf::Vector2f& getRotation() { return m_rotation; }

  /**
   * @brief Obtiene la escala.
   * @return Referencia a la escala.
   */
  sf::Vector2f& getScale() { return m_scale; }

private:
  sf::Vector2f m_position; ///< Posición del objeto.
  sf::Vector2f m_rotation; ///< Rotación del objeto.
  sf::Vector2f m_scale;    ///< Escala del objeto.
};
