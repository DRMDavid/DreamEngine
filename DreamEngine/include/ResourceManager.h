/**
 * @file
 * @brief Clase para la gestión centralizada de recursos.
 * @details Implementa un patrón Singleton para administrar la carga y acceso
 *  a recursos compartidos, como texturas, evitando duplicación y mejorando
 *  la eficiencia de uso de memoria.
 */

#pragma once
#include "Prerequisites.h"
#include "ECS/Texture.h"

 /**
  * @class ResourceManager
  * @brief Gestor de recursos global del motor.
  * @details Permite cargar y recuperar texturas almacenadas en un mapa
  *  interno. Implementa el patrón Singleton para que exista una sola
  *  instancia en toda la aplicación.
  */
class ResourceManager {
public:
  /** @brief Constructor por defecto. */
  ResourceManager() {};

  /** @brief Destructor por defecto. */
  ~ResourceManager() {};

private:
  /** @brief Constructor de copia eliminado (Singleton). */
  ResourceManager(const ResourceManager&) = delete;

  /** @brief Operador de asignación eliminado (Singleton). */
  ResourceManager& operator=(const ResourceManager&) = delete;

public:
  /**
   * @brief Obtiene la instancia única del gestor de recursos.
   * @return Referencia a la instancia única.
   */
  static ResourceManager& getInstance() {
    static ResourceManager instance;
    return instance;
  }

  /**
   * @brief Carga una textura y la almacena en el gestor.
   * @param filename Nombre base del archivo (sin extensión).
   * @param extension Extensión del archivo (por defecto puede ser "png").
   * @return true si la carga fue exitosa, false en caso contrario.
   */
  bool loadTexture(const std::string& filename, const std::string& extension);

  /**
   * @brief Obtiene una textura previamente cargada.
   * @param fileName Nombre base del archivo de textura.
   * @return Puntero compartido a la textura, o nulo si no existe.
   */
  EngineUtilities::TSharedPointer<Texture> getTexture(const std::string& fileName);

private:
  std::unordered_map<std::string, EngineUtilities::TSharedPointer<Texture>> m_textures; ///< Mapa de texturas cargadas.
};
