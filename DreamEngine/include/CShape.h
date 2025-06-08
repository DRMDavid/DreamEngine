#pragma once
#include "Prerequisites.h"

class Window;

/**
 * @class Cshape
 * @brief Encapsula una figura geométrica de SFML y su manipulación.
 */
class
  Cshape {
public:
  Cshape() = default;

  /**
   * @brief Constructor que inicializa con tipo de figura.
   * @param shapeType Tipo de figura.
   */
  Cshape(ShapeType shapeType);

  ~Cshape();

  /**
   * @brief Crea una figura del tipo especificado.
   * @param shapeType Tipo de figura.
   * @return Puntero a la figura creada.
   */
  sf::Shape*
    createShape(ShapeType shapeType);

  /**
   * @brief Actualiza la figura por frame.
   * @param deltaTime Tiempo entre frames.
   */
  void
    update(float deltaTime);

  /**
   * @brief Renderiza la figura en pantalla.
   */
  void
    render();

  /**
   * @brief Establece posición usando coordenadas individuales.
   * @param x Coordenada X.
   * @param y Coordenada Y.
   */
  void
    setPosition(float x, float y);

  /**
   * @brief Establece la posición usando vector.
   * @param position Vector de posición.
   */
  void
    setPosition(const sf::Vector2f& position);

  /**
   * @brief Establece el color de relleno.
   * @param color Color a aplicar.
   */
  void
    setFillColor(const sf::Color& color);

  /**
   * @brief Establece el ángulo de rotación.
   * @param angle Ángulo en grados.
   */
  void
    setRotation(float angle);

  /**
   * @brief Establece la escala de la figura.
   * @param scl Escala 2D.
   */
  void
    setScale(const sf::Vector2f& scl);

  /**
   * @brief Devuelve la figura interna.
   * @return Puntero a sf::Shape.
   */
  sf::Shape*
    getShape() {
    return m_shape;
  }

private:
  sf::Shape* m_shape;         ///< Puntero a la figura de SFML.
  ShapeType m_shapeType;      ///< Tipo de figura creada.
  sf::VertexArray m_line;     ///< Línea de referencia (si aplica).
};
