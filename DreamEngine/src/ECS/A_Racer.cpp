/**
 * @file
 * @brief Implementación de la clase A_Racer.
 * @details Define el comportamiento de actualización para un corredor (Racer),
 *  sincronizando la posición lógica con el componente `Transform` antes de
 *  ejecutar la actualización general del actor.
 */

#include "ECS/A_Racer.h"
#include "ECS/Transform.h"

 /**
  * @brief Constructor de A_Racer.
  * @param name Nombre identificador del corredor.
  */
A_Racer::A_Racer(const std::string& name)
  : Actor(name) {
}

/**
 * @brief Actualiza el estado del corredor.
 * @details Sincroniza la posición lógica (`logicPosition`) con el componente
 *  `Transform` y luego llama a la actualización de la clase base `Actor`.
 * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
 */
void A_Racer::update(float deltaTime) {
  if (auto tr = getComponent<Transform>()) {
    tr->setPosition(sf::Vector2f(logicPosition.x, logicPosition.y));
  }
  Actor::update(deltaTime);
}
