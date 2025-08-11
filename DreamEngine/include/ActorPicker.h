/**
 * @file
 * @brief Utilidad para seleccionar actores con el ratón.
 * @details Permite identificar un actor bajo la posición actual del cursor
 *  del ratón en la ventana, utilizando sus límites globales (`getGlobalBounds`).
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ECS/Actor.h"
#include "CShape.h"

 /**
  * @class ActorPicker
  * @brief Clase utilitaria para selección de actores.
  * @details Implementa un método estático para determinar qué actor se encuentra
  *  bajo el puntero del ratón dentro de la ventana de renderizado.
  */
class ActorPicker {
public:
  /**
   * @brief Devuelve el índice del actor bajo el cursor.
   * @details Recorre todos los actores dados, obtiene su componente `CShape` y
   *  verifica si el punto del ratón en coordenadas del mundo está dentro de sus
   *  límites globales.
   * @param windowPtr Puntero a la ventana SFML.
   * @param actors Vector de punteros compartidos a actores.
   * @return Índice del actor bajo el ratón o -1 si no hay ninguno.
   */
  static int pickActorUnderMouse(
    sf::RenderWindow* windowPtr,
    const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors)
  {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*windowPtr);
    sf::Vector2f mouseWorld = windowPtr->mapPixelToCoords(mousePos);

    for (int i = 0; i < static_cast<int>(actors.size()); ++i) {
      auto shape = actors[i]->getComponent<CShape>();
      if (shape) {
        auto shapePtr = shape->getShapePtr();
        if (shapePtr && shapePtr->getGlobalBounds().contains(mouseWorld)) {
          return i;
        }
      }
    }
    return -1;
  }
};
