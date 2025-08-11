/**
 * @file
 * @brief Clase base para todos los actores en la escena.
 * @details Hereda de `Entity` y añade nombre, manejo de textura y acceso
 *  genérico a componentes. Sirve como clase padre para objetos interactivos
 *  y con comportamiento en el juego.
 */

#pragma once

#include "../Prerequisites.h"
#include "Entity.h"
#include "Cshape.h"
#include "./Transform.h"

 /**
  * @class Actor
  * @brief Representa un objeto interactivo en la escena.
  * @details Es una clase base para entidades que requieren lógica, renderizado
  *  y manipulación de componentes.
  */
class Actor : Entity {
public:
  /** @brief Constructor por defecto. */
  Actor() = default;

  /**
   * @brief Constructor con nombre personalizado.
   * @param actorName Nombre identificador del actor.
   */
  Actor(const std::string& actorName);

  /** @brief Destructor virtual por defecto. */
  virtual ~Actor() = default;

  /** @brief Inicializa el actor y sus componentes. */
  void start() override;

  /**
   * @brief Actualiza la lógica del actor.
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float deltaTime) override;

  /**
   * @brief Renderiza el actor en la ventana proporcionada.
   * @param window Puntero compartido a la ventana de renderizado.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /** @brief Libera recursos y destruye el actor. */
  void destroy() override;

  /**
   * @brief Asigna la textura del actor.
   * @param texture Puntero compartido a la textura.
   */
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

  /**
   * @brief Obtiene el nombre del actor.
   * @return Referencia constante al nombre.
   */
  const std::string& getName() const { return m_name; }

  /**
   * @brief Obtiene un componente del actor por tipo.
   * @tparam T Tipo del componente a obtener.
   * @return Puntero compartido al componente o vacío si no existe.
   */
  template <typename T>
  EngineUtilities::TSharedPointer<T> getComponent();

private:
  std::string m_name = "Actor"; ///< Nombre del actor.
};

/**
 * @brief Implementación de `getComponent` para obtener un componente específico.
 * @tparam T Tipo del componente a buscar.
 * @return Puntero compartido al componente encontrado o vacío si no se halló.
 */
template<typename T>
inline EngineUtilities::TSharedPointer<T> Actor::getComponent() {
  for (auto& component : components) {
    EngineUtilities::TSharedPointer<T> specificComponent
      = component.template dynamic_pointer_cast<T>();
    if (specificComponent) {
      return specificComponent;
    }
  }
  return EngineUtilities::TSharedPointer<T>();
}
