#pragma once

#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>

class Window;

/**
 * @class Transform
 * @brief Componente que almacena transformaciones basicas de una entidad.
 *
 * Gestiona posicion, rotacion y escala usando vectores 2D de SFML.
 * Hereda de Component.
 */
class Transform : public Component {

public:
  /**
   * @brief Constructor por defecto que inicializa la transformacion a valores neutros.
   * Posicion (0,0), rotacion (0,0) y escala (1,1).
   */
  Transform() :
    m_position(0.f, 0.f),
    m_rotation(0.f, 0.f),
    m_scale(1.f, 1.f),
    Component(ComponentType::TRANSFORM) {
  }

  /// Destructor virtual por defecto
  virtual ~Transform() = default;

  /// Metodo llamado al iniciar el componente (override)
  void start() override;

  /**
   * @brief Actualiza la logica del componente (override).
   * @param deltaTime Tiempo delta desde la ultima actualizacion.
   */
  void update(float deltaTime) override;

  /**
   * @brief Renderiza la transformacion (override).
   * @param window Puntero inteligente a la ventana donde renderizar.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /// Metodo para liberar recursos (override)
  void destroy() override;

  // Setters

  /**
   * @brief Establece la posicion del objeto.
   * @param _position Vector 2D con la nueva posicion.
   */
  void setPosition(const sf::Vector2f& _position) {
    m_position = _position;
  }

  /**
   * @brief Establece la rotacion del objeto.
   * @param _rotation Vector 2D con la nueva rotacion.
   */
  void setRotation(const sf::Vector2f& _rotation) {
    m_rotation = _rotation;
  }

  /**
   * @brief Establece la escala del objeto.
   * @param _scale Vector 2D con la nueva escala.
   */
  void setScale(const sf::Vector2f& _scale) {
    m_scale = _scale;
  }

  // Getters

  /**
   * @brief Obtiene la posicion actual.
   * @return Referencia al vector posicion.
   */
  sf::Vector2f& getPosition() {
    return m_position;
  }

  /**
   * @brief Obtiene la rotacion actual.
   * @return Referencia al vector rotacion.
   */
  sf::Vector2f& getRotation() {
    return m_rotation;
  }

  /**
   * @brief Obtiene la escala actual.
   * @return Referencia al vector escala.
   */
  sf::Vector2f& getScale() {
    return m_scale;
  }

private:
  sf::Vector2f m_position; ///< Posicion 2D
  sf::Vector2f m_rotation; ///< Rotacion 2D
  sf::Vector2f m_scale;    ///< Escala 2D
};
