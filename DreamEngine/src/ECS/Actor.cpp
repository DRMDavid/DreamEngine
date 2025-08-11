/**
 * @file
 * @brief Implementación de la clase Actor.
 * @details Define el comportamiento base de un actor en la escena,
 *  incluyendo su inicialización por defecto con componentes esenciales
 *  (`CShape` y `Transform`), y la lógica para renderizado, actualización
 *  y asignación de texturas.
 */

#include "ECS/Actor.h"

 /**
  * @brief Constructor que inicializa un actor con nombre y componentes básicos.
  * @param actorName Nombre identificador del actor.
  * @details Crea y añade un componente `CShape` y un `Transform` al actor.
  */
Actor::Actor(const std::string& actorName) {
  // Asignar nombre
  m_name = actorName;

  // Crear y agregar componente de forma
  EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
  addComponent(shape);

  // Crear y agregar componente de transformación
  EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
  addComponent(transform);
}

/**
 * @brief Renderiza todos los componentes del actor.
 * @param window Puntero compartido a la ventana de renderizado.
 */
void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  for (unsigned int i = 0; i < components.size(); i++) {
    auto component = components[i];
    if (component) {
      component->render(window);
    }
  }
}

/** @brief Inicializa el actor (sin implementación específica por defecto). */
void Actor::start() {
}

/**
 * @brief Actualiza el estado del actor.
 * @details Sincroniza la posición, rotación y escala del componente `CShape`
 *  con los valores actuales del `Transform`.
 * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
 */
void Actor::update(float deltaTime) {
  auto transform = getComponent<Transform>();
  auto shape = getComponent<CShape>();

  if (transform && shape) {
    shape->setPosition(transform->getPosition());
    shape->setRotation(transform->getRotation().x);
    shape->setScale(transform->getScale());
  }
}

/** @brief Destruye y libera los recursos del actor (sin implementación específica). */
void Actor::destroy() {
}

/**
 * @brief Asigna una textura al componente `CShape` del actor.
 * @param texture Puntero compartido a la textura.
 * @details Si la textura es válida, se asigna al `CShape` y se añade como
 *  componente del actor.
 */
void Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  auto shape = getComponent<CShape>();
  if (shape) {
    if (!texture.isNull()) {
      shape->setTexture(texture);
      addComponent(texture);
    }
  }
}
