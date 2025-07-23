#include "ECS/Actor.h"
#include "ECS/Texture.h"

/**
 * @brief Constructor que inicializa un Actor con un nombre.
 *        Agrega automáticamente los componentes CShape y Transform.
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
 * @brief Método para renderizar todos los componentes del Actor.
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
 * @brief Asigna una textura al Actor (la añade como componente y la pasa al CShape).
 * @param texture Puntero compartido a la textura.
 */
void
Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  auto shape = getComponent<CShape>();
  if (shape) {
    if (!texture.isNull()) {
      shape->setTexture(texture);
      addComponent(texture);
    }
  }
}

/**
 * @brief Método llamado al iniciar el Actor.
 *        Actualmente vacío, puede usarse para inicializaciones.
 */
void Actor::start() {
  // Implementar inicializaciones si es necesario
}

/**
 * @brief Actualiza el Actor en cada frame.
 *        Ajusta posición, rotación y escala del shape según el transform.
 * @param deltaTime Tiempo transcurrido desde el último frame (en segundos).
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
 * @brief Método para liberar recursos del Actor.
 *        Actualmente vacío, puede usarse para limpieza.
 */
void Actor::destroy() {
  // Implementar limpieza si es necesario
}
