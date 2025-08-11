/**
 * @file
 * @brief Clase para la gestión de la interfaz gráfica del motor (Engine GUI).
 * @details Implementa herramientas visuales como barra de menús, jerarquía
 *  de actores, inspector de propiedades, consola, gestor de archivos y
 *  resaltado de selección en la ventana de renderizado.
 */

#pragma once
#include "Prerequisites.h"
#include <vector>

class Window;
class Actor;

/**
 * @class EngineGUI
 * @brief Sistema de interfaz gráfica para el motor.
 * @details Ofrece utilidades para inspeccionar, manipular y visualizar
 *  elementos de la escena mediante paneles y controles gráficos.
 */
class EngineGUI {
public:
  /** @brief Constructor por defecto. */
  EngineGUI() = default;

  /** @brief Destructor por defecto. */
  ~EngineGUI() = default;

  /**
   * @brief Inicializa la interfaz gráfica.
   * @param window Puntero compartido a la ventana principal.
   */
  void init(const EngineUtilities::TSharedPointer<Window>& window);

  /**
   * @brief Actualiza la interfaz gráfica.
   * @param window Puntero compartido a la ventana principal.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   */
  void update(const EngineUtilities::TSharedPointer<Window>& window,
    sf::Time deltaTime);

  /**
   * @brief Renderiza la interfaz gráfica.
   * @param window Puntero compartido a la ventana principal.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window);

  /** @brief Libera recursos de la interfaz gráfica. */
  void destroy();

  /**
   * @brief Procesa eventos de la ventana para la interfaz gráfica.
   * @param window Referencia a la ventana SFML.
   * @param event Evento recibido.
   */
  void processEvent(const sf::Window& window, const sf::Event& event);

  /** @brief Configura el estilo visual de la interfaz del motor. */
  void SetupMungoEngineGUIStyle();

  /** @brief Dibuja la barra de menús principal. */
  void menuBar();

  /**
   * @brief Muestra la jerarquía de actores en la escena.
   * @param actors Lista de actores en la escena.
   */
  void hierarchy(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

  /**
   * @brief Muestra y permite editar propiedades del actor seleccionado.
   * @param actors Lista de actores en la escena.
   */
  void inspector(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

  /** @brief Muestra la consola de la interfaz. */
  void console();

  /**
   * @brief Dibuja un contorno resaltado alrededor del actor seleccionado.
   * @param renderWindow Puntero a la ventana de renderizado SFML.
   * @param actors Lista de actores en la escena.
   */
  void drawSelectedOutline(sf::RenderWindow* renderWindow,
    const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

  /**
   * @brief Muestra el panel de gestión de archivos.
   * @param actors Lista de actores en la escena (puede modificarse).
   */
  void fileManagerPanel(std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

  int selectedActorIndex = -1; ///< Índice del actor seleccionado (-1 si ninguno).
};
