/**
 * @file CShape.cpp
 * @brief Implementación de la clase CShape para la creación y manejo de formas gráficas.
 * @details Esta clase encapsula objetos de tipo `sf::Shape` (SFML) y provee
 *  métodos para crearlos, configurarlos y renderizarlos a través del motor.
 */

#include "CShape.h"
#include "Window.h"

 /**
  * @brief Crea una forma según el tipo especificado.
  * @param type Tipo de forma a crear (CIRCLE, RECTANGLE, TRIANGLE, POLYGON).
  * @details
  *  - Inicializa `m_shapePtr` con la forma correspondiente.
  *  - Asigna un color de relleno blanco por defecto.
  *  - Si el tipo no es reconocido, resetea el puntero y emite un error.
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
 * @brief Método llamado al inicio del ciclo de vida del componente.
 * @details Actualmente no realiza ninguna operación.
 */
void CShape::start() {
}

/**
 * @brief Actualiza el estado del componente.
 * @param deltaTime Tiempo transcurrido desde el último frame, en segundos.
 * @details Actualmente no realiza ninguna operación.
 */
void CShape::update(float deltaTime) {
}

/**
 * @brief Renderiza la forma en la ventana especificada.
 * @param window Puntero inteligente a la ventana destino.
 * @details Llama a `window->draw()` si `m_shapePtr` está inicializado.
 */
void CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (m_shapePtr) {
    window->draw(*m_shapePtr);
  }
}

/**
 * @brief Libera los recursos del componente.
 * @details Actualmente no realiza ninguna operación.
 */
void CShape::destroy() {
}

/**
 * @brief Establece la posición de la forma.
 * @param x Posición en el eje X.
 * @param y Posición en el eje Y.
 * @note Si `m_shapePtr` no está inicializado, se muestra un error.
 */
void CShape::setPosition(float x, float y) {
  if (m_shapePtr) {
    m_shapePtr->setPosition({ x, y });
  }
  else {
    ERROR("CShape", "setPosition", "Shape no inicializado");
  }
}

/**
 * @brief Establece la posición de la forma.
 * @param position Vector con las coordenadas (x, y).
 * @note Si `m_shapePtr` no está inicializado, se muestra un error.
 */
void CShape::setPosition(const sf::Vector2f& position) {
  if (m_shapePtr) {
    m_shapePtr->setPosition(position);
  }
  else {
    ERROR("CShape", "setPosition", "Shape no inicializado");
  }
}

/**
 * @brief Asigna el color de relleno de la forma.
 * @param color Color en formato SFML.
 * @note Si `m_shapePtr` no está inicializado, se muestra un error.
 */
void CShape::setFillColor(const sf::Color& color) {
  if (m_shapePtr) m_shapePtr->setFillColor(color);
  else ERROR("CShape", "setFillColor", "Shape no inicializado");
}

/**
 * @brief Rota la forma.
 * @param angle Ángulo en grados.
 * @note Si `m_shapePtr` no está inicializado, se muestra un error.
 */
void CShape::setRotation(float angle) {
  if (m_shapePtr) m_shapePtr->setRotation(sf::degrees(angle));
  else ERROR("CShape", "setRotation", "Shape no inicializado");
}

/**
 * @brief Escala la forma.
 * @param scale Vector con factor de escala en X e Y.
 * @note Si `m_shapePtr` no está inicializado, se muestra un error.
 */
void CShape::setScale(const sf::Vector2f& scale) {
  if (m_shapePtr) m_shapePtr->setScale(scale);
  else ERROR("CShape", "setScale", "Shape no inicializado");
}

/**
 * @brief Asigna una textura a la forma.
 * @param texture Puntero inteligente a la textura.
 * @details Si la textura no es nula, se aplica a `m_shapePtr`.
 */
void CShape::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  if (!texture.isNull()) {
    m_shapePtr->setTexture(&texture->getTexture());
  }
}
