/**
 * @file
 * @brief Funciones para serializar y deserializar actores.
 * @details Proporciona utilidades para guardar y cargar el estado de una
 *  lista de actores desde/ hacia un archivo.
 */

#pragma once
#include "ECS/Actor.h"
#include "Prerequisites.h"

 /**
  * @namespace ActorSerializer
  * @brief Espacio de nombres para funciones de serialización de actores.
  */
namespace ActorSerializer {

  /**
   * @brief Guarda una lista de actores en un archivo.
   * @param filename Ruta y nombre del archivo de destino.
   * @param actors Vector de punteros compartidos a actores.
   * @return true si la operación fue exitosa, false en caso contrario.
   */
  bool saveActorsToFile(
    const std::string& filename,
    const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors
  );

  /**
   * @brief Carga una lista de actores desde un archivo.
   * @param filename Ruta y nombre del archivo de origen.
   * @param actors Vector donde se almacenarán los actores cargados.
   * @return true si la operación fue exitosa, false en caso contrario.
   */
  bool loadActorsFromFile(
    const std::string& filename,
    std::vector<EngineUtilities::TSharedPointer<Actor>>& actors
  );
}
