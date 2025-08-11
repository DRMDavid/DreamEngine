/**
 * @file
 * @brief Componente para representar y manipular formas gráficas.
 * @details Permite crear y configurar formas (`sf::Shape`) en SFML, asignarles
 *  texturas y propiedades visuales, y renderizarlas en una ventana.
 */

#pragma once
#include "Prerequisites.h"
#include <ECS/Component.h>
#include "ECS/Texture.h"

class Window;

/**
 * @class CShape
 * @brief Componente gráfico que gestiona una forma SFML.
 * @details Soporta la creación de distintos tipos de forma (`ShapeType`),
 *  configuración de propiedades visuales, texturas y renderizado.
 */
class CShape : public Component {
public:
  /** @brief Constructor por defecto. */
  CShape() = default;

  /**
   * @brief Constructor que inicializa con un tipo de forma.
   * @param shapeType Tipo de forma a crear.
   */
  CShape(ShapeType shapeType) :
    m_shapePtr(nullptr),
    m_shapeType(ShapeType::EMPTY),
    Component(ComponentType::SHAPE) {
  }

  /** @brief Destructor virtual por defecto. */
  virtual ~CShape() = default;

  /**
   * @brief Crea la forma con el tipo especificado.
   * @param shapeType Tipo de forma a crear.
   */
  void createShape(ShapeType shapeType);

  /** @brief Inicializa el componente (sin implementación específica por defecto). */
  void start() override;

  /**
   * @brief Actualiza la lógica de la forma.
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float deltaTime) override;

  /**
   * @brief Renderiza la forma en la ventana.
   * @param window Puntero compartido a la ventana donde se dibujará.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /** @brief Libera los recursos asociados a la forma. */
  void destroy() override;

  /**
   * @brief Asigna la posición de la forma.
   * @param x Coordenada X.
   * @param y Coordenada Y.
   */
  void setPosition(float x, float y);

  /**
   * @brief Asigna la posición de la forma.
   * @param position Vector de posición (X,Y).
   */
  void setPosition(const sf::Vector2f& position);

  /**
   * @brief Asigna el color de relleno de la forma.
   * @param color Color de relleno.
   */
  void setFillColor(const sf::Color& color);

  /**
   * @brief Asigna la rotación de la forma.
   * @param angle Ángulo en grados.
   */
  void setRotation(float angle);

  /**
   * @brief Asigna la escala de la forma.
   * @param scl Vector de escala (X,Y).
   */
  void setScale(const sf::Vector2f& scl);

  /**
   * @brief Asigna una textura a la forma.
   * @param texture Puntero compartido a la textura.
   */
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

  /**
   * @brief Obtiene el puntero a la forma SFML.
   * @return Puntero compartido a `sf::Shape`.
   */
  EngineUtilities::TSharedPointer<sf::Shape> getShapePtr() { return m_shapePtr; }

private:
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr; ///< Puntero a la forma SFML.
  ShapeType m_shapeType; ///< Tipo de forma actual.
  sf::VertexArray* m_line; ///< Posible referencia a una forma de línea.
};
