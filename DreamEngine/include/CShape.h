#pragma once
#include "Prerequisites.h"
#include "ECS\Component.h"

class Window;

/**
 * @class CShape
 * @brief Componente que representa una forma grafica para renderizar.
 *
 * Hereda de Component y administra un objeto sf::Shape para representar
 * figuras geometricas en pantalla.
 */
class CShape : public Component {
public:
  /// Constructor por defecto
  CShape() = default;

  /**
   * @brief Constructor que inicializa el tipo de forma.
   * @param shapeType Tipo de forma a crear.
   */
  CShape(ShapeType shapeType) :
    m_shapePtr(nullptr),
    m_shapeType(ShapeType::EMPTY),
    Component(ComponentType::SHAPE) {
  }

  /// Destructor virtual por defecto
  virtual ~CShape() = default;

  /**
   * @brief Crea una forma del tipo especificado.
   * @param shapeType Tipo de forma a crear.
   */
  void createShape(ShapeType shapeType);

  /// Metodo llamado al iniciar el componente (override)
  void start() override;

  /**
   * @brief Actualiza la logica de la forma.
   * @param deltaTime Tiempo delta desde la ultima actualizacion.
   */
  void update(float deltaTime) override;

  /**
   * @brief Renderiza la forma en la ventana dada.
   * @param window Puntero inteligente a la ventana donde renderizar.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /// Metodo para liberar recursos (override)
  void destroy() override;

  /**
   * @brief Establece la posicion de la forma (x,y).
   * @param x Posicion en x.
   * @param y Posicion en y.
   */
  void setPosition(float x, float y);

  /**
   * @brief Establece la posicion de la forma usando sf::Vector2f.
   * @param position Vector con la posicion (x,y).
   */
  void setPosition(const sf::Vector2f& position);

  /**
   * @brief Cambia el color de relleno de la forma.
   * @param color Color de relleno.
   */
  void setFillColor(const sf::Color& color);

  /**
   * @brief Establece la rotacion de la forma en grados.
   * @param angle Angulo de rotacion.
   */
  void setRotation(float angle);

  /**
   * @brief Establece la escala de la forma.
   * @param scl Vector con escala en x y y.
   */
  void setScale(const sf::Vector2f& scl);

private:
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr; ///< Puntero inteligente a la forma SFML
  ShapeType m_shapeType; ///< Tipo de forma
  sf::VertexArray* m_line; ///< Puntero a vertices para lineas (si aplica)
};
