#include "ECS/Actor.h"

/**
 * @brief Constructor que inicializa un Actor con un nombre.
 *        Agrega automaticamente los componentes CShape y Transform.
 * @param actorName Nombre del actor.
 */
Actor::Actor(const std::string& actorName) {
  // Configura el nombre del actor
  m_name = actorName;

  // Crea y agrega el componente CShape
  EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
  addComponent(shape);

  // Crea y agrega el componente Transform
  EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
  addComponent(transform);
}

/**
 * @brief Metodo para renderizar todos los componentes del Actor.
 * @param window Puntero compartido a la ventana donde se dibuja.
 */
void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  for (unsigned int i = 0; i < components.size(); i++) {
    auto component = components[i];
    if (component) {
      component->render(window);
    }
  }
}

/**
 * @brief Metodo llamado al iniciar el Actor.
 *        Actualmente vacio, puede usarse para inicializaciones.
 */
void Actor::start() {
  // Implementar inicializaciones si es necesario
}

/**
 * @brief Actualiza el Actor en cada frame.
 *        Actualiza posicion, rotacion y escala del shape segun el transform.
 * @param deltaTime Tiempo transcurrido desde el ultimo frame (en segundos).
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

/**
 * @brief Metodo para liberar recursos del Actor.
 *        Actualmente vacio, puede usarse para limpieza.
 */
void Actor::destroy() {
  // Implementar limpieza si es necesario
}
