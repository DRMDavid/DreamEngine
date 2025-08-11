/**
 * @file
 * @brief Clase base para todas las entidades del motor.
 * @details Define la interfaz del ciclo de vida y gestión de componentes
 *  para cualquier objeto en la escena. Las entidades pueden tener múltiples
 *  componentes que definen su comportamiento y apariencia.
 */

#pragma once

#include "../Prerequisites.h"
#include "Component.h"

class Window;

/**
 * @class Entity
 * @brief Representa una entidad genérica en la escena.
 * @details Proporciona un sistema de componentes para añadir, obtener y
 *  gestionar funcionalidades específicas.
 */
class Entity {
public:
  /** @brief Destructor virtual por defecto. */
  virtual ~Entity() = default;

  /** @brief Inicializa la entidad y sus componentes. */
  virtual void start() = 0;

  /**
   * @brief Actualiza la lógica de la entidad.
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  virtual void update(float deltaTime) = 0;

  /**
   * @brief Renderiza la entidad.
   * @param window Puntero compartido a la ventana de renderizado.
   */
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /** @brief Libera recursos y destruye la entidad. */
  virtual void destroy() = 0;

  /**
   * @brief Añade un componente a la entidad.
   * @tparam T Tipo de componente (debe heredar de `Component`).
   * @param component Puntero compartido al componente.
   * @note El componente se almacena como puntero a `Component` para
   *  permitir polimorfismo.
   */
  template<typename T>
  void addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value,
      "T must be derived from Component");
    components.push_back(component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Obtiene un componente de la entidad por tipo.
   * @tparam T Tipo del componente a obtener.
   * @return Puntero compartido al componente o vacío si no existe.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T> getComponent() {
    for (auto& component : components) {
      EngineUtilities::TSharedPointer<T> specificComponent =
        component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

protected:
  bool isActive = true; ///< Indica si la entidad está activa.
  uint32_t id = 0; ///< Identificador único de la entidad.
  std::vector<EngineUtilities::TSharedPointer<Component>> components; ///< Lista de componentes.
};
