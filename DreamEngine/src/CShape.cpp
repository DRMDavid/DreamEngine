#include "CShape.h"
#include "Window.h"

/**
 * @brief Crea la forma segun el tipo especificado.
 *        Soporta circulo, rectangulo, triangulo y poligono.
 * @param type Tipo de forma a crear (ShapeType).
 */
void CShape::createShape(ShapeType type) {
  m_shapeType = type;
  switch (type) {
  case ShapeType::CIRCLE: {
    auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
    circleSP->setFillColor(sf::Color::White);
    m_shapePtr = circleSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::RECTANGLE: {
    auto rectSP = EngineUtilities::MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
    rectSP->setFillColor(sf::Color::White);
    m_shapePtr = rectSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::TRIANGLE: {
    auto triSP = EngineUtilities::MakeShared<sf::ConvexShape>(3);
    triSP->setPoint(0, { 0,0 });
    triSP->setPoint(1, { 50,100 });
    triSP->setPoint(2, { 100,0 });
    triSP->setFillColor(sf::Color::White);
    m_shapePtr = triSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::POLYGON: {
    auto polySP = EngineUtilities::MakeShared<sf::ConvexShape>(5);
    polySP->setPoint(0, { 0,0 });
    polySP->setPoint(1, { 50,100 });
    polySP->setPoint(2, { 100,0 });
    polySP->setPoint(3, { 75,-50 });
    polySP->setPoint(4, { -25,-50 });
    polySP->setFillColor(sf::Color::White);
    m_shapePtr = polySP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  default:
    m_shapePtr.reset();
    ERROR("CShape", "createShape", "Tipo desconocido");
    return;
  }
}

/**
 * @brief Metodo llamado al iniciar el componente.
 *        Actualmente sin implementacion.
 */
void CShape::start() {
}

/**
 * @brief Actualiza el estado del componente.
 * @param deltaTime Tiempo transcurrido desde la ultima actualizacion.
 *        Actualmente sin implementacion.
 */
void CShape::update(float deltaTime) {
}

/**
 * @brief Renderiza la forma en la ventana indicada.
 * @param window Puntero inteligente a la ventana donde dibujar.
 */
void CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (m_shapePtr) {
    window->draw(*m_shapePtr);
  }
}

/**
 * @brief Destruye y libera recursos asociados a la forma.
 */
void CShape::destroy() {
  m_shapePtr.reset();
}

/**
 * @brief Establece la posicion de la forma con coordenadas x, y.
 * @param x Coordenada horizontal.
 * @param y Coordenada vertical.
 */
void CShape::setPosition(float x, float y) {
  if (m_shapePtr) m_shapePtr->setPosition(x, y);
  else ERROR("CShape", "setPosition", "Shape no inicializado");
}

/**
 * @brief Establece la posicion de la forma con un vector 2D.
 * @param pos Vector con la posicion deseada.
 */
void CShape::setPosition(const sf::Vector2f& pos) {
  if (m_shapePtr) m_shapePtr->setPosition(pos);
  else ERROR("CShape", "setPosition", "Shape no inicializado");
}

/**
 * @brief Cambia el color de relleno de la forma.
 * @param color Color a establecer.
 */
void CShape::setFillColor(const sf::Color& color) {
  if (m_shapePtr) m_shapePtr->setFillColor(color);
  else ERROR("CShape", "setFillColor", "Shape no inicializado");
}

/**
 * @brief Establece la rotacion (angulo) de la forma.
 * @param angle Angulo en grados.
 */
void CShape::setRotation(float angle) {
  if (m_shapePtr) m_shapePtr->setRotation(angle);
  else ERROR("CShape", "setRotation", "Shape no inicializado");
}

/**
 * @brief Establece la escala de la forma mediante un vector 2D.
 * @param scale Vector con factores de escala en x y y.
 */
void CShape::setScale(const sf::Vector2f& scale) {
  if (m_shapePtr) m_shapePtr->setScale(scale);
  else ERROR("CShape", "setScale", "Shape no inicializado");
}
