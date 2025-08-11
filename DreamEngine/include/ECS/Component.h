/**
 * @file
 * @brief Clase base para todos los componentes del motor.
 * @details Define la interfaz común y el tipo de componente. Sirve como
 *  clase padre para elementos funcionales como transformaciones, sprites,
 *  renderizadores, físicas, audio, formas y texturas.
 */

#pragma once

#include "../Prerequisites.h"

class Window;

/**
 * @enum ComponentType
 * @brief Tipos de componentes soportados por el motor.
 */
enum ComponentType {
	NONE = 0,      ///< Sin tipo asignado.
	TRANSFORM = 1, ///< Componente de transformación (posición, rotación, escala).
	SPRITE = 2,    ///< Componente para manejar sprites.
	RENDERER = 3,  ///< Componente encargado del renderizado.
	PHYSICS = 4,   ///< Componente de físicas.
	AUDIOSOURCE = 5, ///< Componente de audio.
	SHAPE = 6,     ///< Componente de forma geométrica.
	TEXTURE = 7    ///< Componente de textura.
};

/**
 * @class Component
 * @brief Clase base abstracta para todos los componentes.
 * @details Define métodos virtuales puros para el ciclo de vida del
 *  componente: inicio, actualización, renderizado y destrucción.
 */
class Component {
public:
	/** @brief Constructor por defecto. */
	Component() = default;

	/**
	 * @brief Constructor que asigna el tipo de componente.
	 * @param type Tipo del componente según `ComponentType`.
	 */
	Component(const ComponentType type) : m_type(type) {}

	/** @brief Destructor virtual por defecto. */
	virtual ~Component() = default;

	/** @brief Llamado al iniciar el componente. */
	virtual void start() = 0;

	/**
	 * @brief Llamado cada frame para actualizar la lógica del componente.
	 * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
	 */
	virtual void update(float deltaTime) = 0;

	/**
	 * @brief Llamado para renderizar el componente.
	 * @param window Puntero compartido a la ventana donde se dibuja.
	 */
	virtual void render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

	/** @brief Llamado para liberar recursos y destruir el componente. */
	virtual void destroy() = 0;

	/**
	 * @brief Obtiene el tipo del componente.
	 * @return Tipo según `ComponentType`.
	 */
	ComponentType getType() const { return m_type; }

protected:
	ComponentType m_type = ComponentType::NONE; ///< Tipo del componente.
};
