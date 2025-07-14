#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "CShape.h"
#include "ECS/Actor.h"

/**
 * @class BaseApp
 * @brief Clase principal que gestiona el ciclo de vida de la aplicacion.
 *
 * Contiene punteros inteligentes a Window, CShape y Actor,
 * y controla la inicializacion, actualizacion, renderizado y destruccion.
 */
class BaseApp {
public:
  /// Constructor por defecto
  BaseApp() = default;

  /// Destructor
  ~BaseApp();

  /**
   * @brief Metodo principal que ejecuta el ciclo de la aplicacion.
   * @return int Codigo de salida.
   */
  int run();

  /**
   * @brief Inicializa los componentes necesarios para la aplicacion.
   * @return true si la inicializacion fue exitosa.
   */
  bool init();

  /**
   * @brief Actualiza la logica del programa en cada ciclo.
   */
  void update();

  /**
   * @brief Renderiza los elementos en pantalla.
   */
  void render();

  /**
   * @brief Libera los recursos y limpia antes de cerrar.
   */
  void destroy();

private:
  EngineUtilities::TSharedPointer<Window> m_windowPtr; ///< Puntero inteligente a ventana

  EngineUtilities::TSharedPointer<CShape> m_shapePtr; ///< Puntero inteligente a figura

  EngineUtilities::TSharedPointer<Actor> m_ACircle; ///< Puntero inteligente a actor circulo
};
