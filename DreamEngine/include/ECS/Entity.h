#pragma once
#include "../Prerequisites.h"
#include "Component.h"

class Window;

/**
 * @class Entity
 * @brief Clase base abstracta para entidades del sistema.
 *
 * Una entidad puede tener multiples componentes y maneja su ciclo de vida.
 */
class Entity {
public:

  /**
   * @brief Destructor virtual por defecto.
   */
  virtual ~Entity() = default;

  /**
   * @brief Metodo virtual puro para iniciar la entidad.
   */
  virtual void start() = 0;

  /**
   * @brief Metodo virtual puro para actualizar la entidad.
   * @param deltaTime Tiempo transcurrido desde la ultima actualizacion.
   */
  virtual void update(float deltaTime) = 0;

  /**
   * @brief Metodo virtual puro para renderizar la entidad.
   * @param window Puntero inteligente a la ventana donde se debe dibujar.
   */
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /**
   * @brief Metodo virtual puro para destruir o limpiar recursos de la entidad.
   */
  virtual void destroy() = 0;

  /**
   * @brief Agrega un componente a la entidad.
   * @tparam T Tipo del componente a agregar. Debe derivar de Component.
   * @param component Puntero inteligente al componente a agregar.
   */
  template<typename T>
  void addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
    components.push_back(component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Obtiene un componente especifico de la entidad.
   * @tparam T Tipo del componente a obtener.
   * @return Puntero inteligente al componente solicitado o nulo si no existe.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T> getComponent() {
    for (auto& component : components) {
      EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

protected:
  bool isActive; ///< Indica si la entidad esta activa.
  uint32_t id; ///< Identificador unico de la entidad.
  std::vector<EngineUtilities::TSharedPointer<Component>> components; ///< Lista de componentes de la entidad.
};
