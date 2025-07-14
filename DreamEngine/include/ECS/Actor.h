#pragma once
#include "../Prerequisites.h"
#include "Entity.h"
#include "Cshape.h"
#include "../Transform.h"

/**
 * @class Actor
 * @brief Representa un actor en la escena que hereda de Entity.
 *
 * La clase Actor puede contener componentes y tiene un nombre identificador.
 * Implementa los metodos basicos de ciclo de vida: start, update, render y destroy.
 */
class Actor : Entity {
public:
  /**
   * @brief Constructor por defecto.
   */
  Actor() = default;

  /**
   * @brief Constructor que recibe un nombre para el actor.
   * @param actorName Nombre asignado al actor.
   */
  Actor(const std::string& actorName);

  /**
   * @brief Destructor virtual por defecto.
   */
  virtual ~Actor() = default;

  /**
   * @brief Metodo llamado al iniciar el actor.
   */
  void start() override;

  /**
   * @brief Metodo para actualizar el estado del actor.
   * @param deltaTime Tiempo transcurrido desde la ultima actualizacion.
   */
  void update(float deltaTime) override;

  /**
   * @brief Metodo para renderizar el actor.
   * @param window Puntero inteligente a la ventana donde se dibuja.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /**
   * @brief Metodo para destruir o limpiar recursos del actor.
   */
  void destroy() override;

  /**
   * @brief Obtiene un componente especifico asociado al actor.
   * @tparam T Tipo de componente a obtener.
   * @return Puntero inteligente al componente encontrado o puntero nulo si no existe.
   */
  template <typename T>
  EngineUtilities::TSharedPointer<T> getComponent();

private:
  /// Nombre del actor
  std::string m_name = "Actor";
};

template<typename T>
inline EngineUtilities::TSharedPointer<T> Actor::getComponent() {
  for (auto& component : components) {
    EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
    if (specificComponent) {
      return specificComponent;
    }
  }
  return EngineUtilities::TSharedPointer<T>();
}
