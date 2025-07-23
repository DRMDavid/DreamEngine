#include "BaseApp.h"
#include <ECS/Actor.h>
#include "SourceManager.h"  // Asegúrate de que este es el nombre correcto


BaseApp::~BaseApp() {
}

int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp",
      "run",
      "Initialization failed, check method validations");
    return -1;
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents();
    update();
    render();
  }

  destroy();
  return 0;
}

bool BaseApp::init() {
  // Crear ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "DreamEngine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer");
    return false;
  }

  // Crear Actor círculo
  m_ACircle = EngineUtilities::MakeShared<Actor>("Circle Actor");
  if (!m_ACircle) {
    ERROR("BaseApp", "init", "Failed to create Circle Actor");
    return false;
  }
  m_ACircle->getComponent<CShape>()->createShape(CIRCLE);
  m_ACircle->getComponent<CShape>()->setFillColor(sf::Color::White);
  m_ACircle->getComponent<Transform>()->setPosition({ 100.f, 150.f });

  // Cargar textura para el actor
  auto& resourceMan = ResourceManager::getInstance();
  if (!resourceMan.loadTexture("Assets/Yoshi", "png")) {
    MESSAGE("BaseApp", "init", "Can't load the texture");
    // Si quieres seguir sin textura, comenta el return y deja que use la por defecto
    return false;
  }
  m_ACircle->setTexture(resourceMan.getTexture("Assets/Yoshi"));

  // Definir waypoints
  m_waypoints = {
      {600.f, 150.f},
      {660.f, 330.f},
      {480.f, 220.f},
      {720.f, 220.f},
      {540.f, 330.f},
      {600.f, 150.f}
  };

  /*/ Crear líneas entre waypoints para visualizar la estrella
  for (size_t i = 0; i + 1 < m_waypoints.size(); ++i) {
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = m_waypoints[i];
    line[0].color = sf::Color::Yellow;
    line[1].position = m_waypoints[i + 1];
    line[1].color = sf::Color::Yellow;
    m_waypointLines.push_back(line);
  }*/

  m_currentWaypointIndex = 0;
  return true;
}

void BaseApp::update() {
  if (m_windowPtr) {
    m_windowPtr->update();
  }

  if (m_ACircle) {
    m_ACircle->update(m_windowPtr->deltaTime.asSeconds());

    if (m_currentWaypointIndex < m_waypoints.size()) {
      sf::Vector2f targetPos = m_waypoints[m_currentWaypointIndex];
      auto currentPos = m_ACircle->getComponent<Transform>()->getPosition();

      float dx = targetPos.x - currentPos.x;
      float dy = targetPos.y - currentPos.y;
      float distance = std::sqrt(dx * dx + dy * dy);

      if (distance < 10.f) {
        // Avanzar al siguiente waypoint
        ++m_currentWaypointIndex;
      }
      else {
        // Buscar el siguiente waypoint
        m_ACircle->getComponent<Transform>()
          ->seek(targetPos, 100.f, m_windowPtr->deltaTime.asSeconds(), 10.f);
      }
    }
  }
}

void BaseApp::render() {
  if (!m_windowPtr) return;

  m_windowPtr->clear();

  // Renderiza líneas de waypoints
  for (auto& line : m_waypointLines) {
    m_windowPtr->draw(line);
  }

  // Renderiza actor
  if (m_ACircle) {
    m_ACircle->render(m_windowPtr);
  }

  m_windowPtr->display();
}

void BaseApp::destroy() {
  // Aquí podrías limpiar recursos si fuera necesario
}