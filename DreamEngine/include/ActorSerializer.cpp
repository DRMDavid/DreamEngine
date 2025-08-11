/**
 * @file
 * @brief Implementación del módulo ActorSerializer.
 * @details Proporciona funciones para guardar y cargar actores (Actor) en/desde un archivo de texto.
 * El formato utilizado es CSV simple:
 * @code
 * nombre,posX,posY,scaleX,scaleY
 * @endcode
 */

#include "ActorSerializer.h"
#include <fstream>
#include <sstream>

 /**
  * @brief Guarda un conjunto de actores en un archivo.
  * @param filename Ruta y nombre del archivo donde guardar.
  * @param actors Vector de punteros compartidos a actores.
  * @return true si se guardó correctamente, false en caso contrario.
  *
  * @details El formato de guardado es una línea por actor:
  * @code
  * nombre,posX,posY,scaleX,scaleY
  * @endcode
  * Si un actor no tiene componente Transform, no se guarda su posición ni escala.
  */
bool ActorSerializer::saveActorsToFile(
  const std::string& filename,
  const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors)
{
  std::ofstream file(filename);
  if (!file.is_open()) return false;

  for (auto& actor : actors) {
    file << actor->getName() << ","; // Nombre del actor
    auto transform = actor->getComponent<Transform>();
    if (transform) {
      auto pos = transform->getPosition();
      auto scale = transform->getScale();
      file << pos.x << "," << pos.y << ","
        << scale.x << "," << scale.y << "\n";
    }
  }

  file.close();
  return true;
}

/**
 * @brief Carga un conjunto de actores desde un archivo.
 * @param filename Ruta y nombre del archivo a cargar.
 * @param actors Vector donde se almacenarán los actores cargados.
 * @return true si la carga fue exitosa, false en caso contrario.
 *
 * @details Se espera un formato CSV como el generado por saveActorsToFile().
 * Cada línea define un actor con su nombre, posición y escala.
 * Si un valor no es válido, la línea se omite.
 */
bool ActorSerializer::loadActorsFromFile(
  const std::string& filename,
  std::vector<EngineUtilities::TSharedPointer<Actor>>& actors)
{
  std::ifstream file(filename);
  if (!file.is_open()) return false;

  actors.clear();
  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string name;
    float px, py, sx, sy;

    if (std::getline(ss, name, ',') &&
      (ss >> px) && ss.ignore(1) &&
      (ss >> py) && ss.ignore(1) &&
      (ss >> sx) && ss.ignore(1) &&
      (ss >> sy))
    {
      auto actor = EngineUtilities::MakeShared<Actor>(name);
      actor->getComponent<Transform>()->setPosition(sf::Vector2f(px, py));
      actor->getComponent<Transform>()->setScale(sf::Vector2f(sx, sy));
      actors.push_back(actor);
    }
  }

  file.close();
  return true;
}
