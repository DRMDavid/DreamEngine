#pragma once
#include "../Prerequisites.h"

class Window;

/**
 * @enum ComponentType
 * @brief Tipos posibles de componentes en el sistema.
 */
enum ComponentType {
  NONE = 0,       /**< Sin tipo */
  TRANSFORM = 1,  /**< Componente de transformacion (posicion, rotacion, escala) */
  SPRITE = 2,     /**< Componente de sprite para graficos */
  RENDERER = 3,   /**< Componente de renderizado */
  PHYSICS = 4,    /**< Componente de fisica */
  AUDIOSOURCE = 5,/**< Componente para audio */
  SHAPE = 6,      /**< Componente para formas geometricas */
  TEXTURE = 7     /**< Componente de textura */
};

/**
 * @class Component
 * @brief Clase base abstracta para componentes que se pueden agregar a entidades o actores.
 *
 * Define la interfaz comun para ciclo de vida y tipo del componente.
 */
class Component {
public:
  /**
   * @brief Constructor por defecto.
   */
  Component() = default;

  /**
   * @brief Constructor con tipo especifico de componente.
   * @param type Tipo de componente asignado.
   */
  Component(const ComponentType type) : m_type(type) {}

  /**
   * @brief Destructor virtual por defecto.
   */
  virtual ~Component() = default;

  /**
   * @brief Metodo virtual puro llamado al iniciar el componente.
   */
  virtual void start() = 0;

  /**
   * @brief Metodo virtual puro para actualizar el componente.
   * @param deltaTime Tiempo transcurrido desde la ultima actualizacion.
   */
  virtual void update(float deltaTime) = 0;

  /**
   * @brief Metodo virtual puro para renderizar el componente.
   * @param window Puntero inteligente a la ventana donde se debe dibujar.
   */
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /**
   * @brief Metodo virtual puro para destruir o limpiar recursos del componente.
   */
  virtual void destroy() = 0;

  /**
   * @brief Obtiene el tipo del componente.
   * @return Tipo enumerado ComponentType.
   */
  ComponentType getType() const { return m_type; }

protected:
  /// Tipo del componente almacenado
  ComponentType m_type;
};
