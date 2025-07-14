#include "BaseApp.h"

/**
 * @brief Destructor de BaseApp.
 *        Actualmente no realiza ninguna accion especifica.
 */
BaseApp::~BaseApp() {
}

/**
 * @brief Metodo principal que ejecuta el ciclo del programa.
 *        Inicializa el sistema, procesa eventos, actualiza y renderiza hasta cerrar la ventana.
 * @return int Codigo de estado (0 en ejecucion correcta).
 */
int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp",
      "run",
      "Inicializacion fallo, verificar validaciones en el metodo init");
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents();
    update();
    render();
  }

  destroy();
  return 0;
}

/**
 * @brief Inicializa los recursos principales de la aplicacion.
 *        Crea ventana, un shape simple y un actor con shape y transform.
 * @return bool True si la inicializacion fue correcta, false en caso contrario.
 */
bool BaseApp::init() {
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "DreamEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp",
      "init",
      "Fallo al crear el puntero de la ventana, revisar asignacion de memoria");
    return false;
  }

  // Crear shape simple (circulo verde)
  m_shapePtr = EngineUtilities::MakeShared<CShape>();
  if (m_shapePtr) {
    m_shapePtr->createShape(ShapeType::CIRCLE);
    m_shapePtr->setFillColor(sf::Color::Green);
    m_shapePtr->setPosition(200.f, 150.f);
  }

  // Crear actor con shape (circulo rojo) y transform (posicion inicial)
  m_ACircle = EngineUtilities::MakeShared<Actor>("Circle Actor");
  if (m_ACircle) {
    m_ACircle->getComponent<CShape>()->createShape(CIRCLE);
    m_ACircle->getComponent<CShape>()->setFillColor(sf::Color::Red);
    m_ACircle->getComponent<Transform>()->setPosition(sf::Vector2(100.f, 150.f));
  }

  return true;
}

/**
 * @brief Actualiza el estado de la aplicacion y los actores.
 *        Por ahora solo actualiza el actor circulo si existe.
 */
void BaseApp::update() {
  if (!m_ACircle.isNull()) {
    m_ACircle->update(0); // deltaTime=0 por ahora
  }
}

/**
 * @brief Renderiza la escena en la ventana.
 *        Dibuja el shape simple y el actor circulo.
 */
void BaseApp::render() {
  if (!m_windowPtr) {
    return;
  }
  m_windowPtr->clear();

  if (m_shapePtr) {
    m_shapePtr->render(m_windowPtr);
  }

  if (m_ACircle) {
    m_ACircle->render(m_windowPtr);
  }

  m_windowPtr->display();
}

/**
 * @brief Libera recursos y limpia antes de cerrar la aplicacion.
 *        Actualmente no realiza acciones especificas.
 */
void BaseApp::destroy() {
}
