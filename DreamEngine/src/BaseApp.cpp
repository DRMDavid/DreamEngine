#include "BaseApp.h"
#include <ECS/Actor.h>

BaseApp::~BaseApp() {
}

int
BaseApp::run() {
	if (!init()) {
		ERROR("BaseApp",
			"run",
			"Initializes result on a false statemente, check method validations");
	}

	while (m_windowPtr->isOpen()) {
		m_windowPtr->handleEvents();
		update();
		render();
	}

	destroy();
	return 0;
}

bool
BaseApp::init() {
	m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "DreamEngine");
	if (!m_windowPtr) {
		ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
		return false;
	}

	// Crear Actor círculo
	m_ACircle = EngineUtilities::MakeShared<Actor>("Circle Actor");
	if (m_ACircle) {
		m_ACircle->getComponent<CShape>()->createShape(CIRCLE);
		m_ACircle->getComponent<CShape>()->setFillColor(sf::Color::Magenta);
		m_ACircle->getComponent<Transform>()->setPosition(sf::Vector2f(100.f, 150.f));
	}

	// Definir Waypoints
	m_waypoints.push_back(sf::Vector2f(600.f, 150.f)); // Punto superior 
	m_waypoints.push_back(sf::Vector2f(660.f, 330.f)); // Punto inferior derecho 
	m_waypoints.push_back(sf::Vector2f(480.f, 220.f)); // Punto izquierdo 
	m_waypoints.push_back(sf::Vector2f(720.f, 220.f)); // Punto derecho 
	m_waypoints.push_back(sf::Vector2f(540.f, 330.f)); // Punto inferior izquierdo 
	m_waypoints.push_back(sf::Vector2f(600.f, 150.f)); // Regresa al inicio

	m_currentWaypointIndex = 0;

	return true;
}

void BaseApp::update() {
	if (!m_windowPtr.isNull()) {
		m_windowPtr->update();
	}

	if (!m_ACircle.isNull()) {
		m_ACircle->update(m_windowPtr->deltaTime.asSeconds());

		if (m_currentWaypointIndex < m_waypoints.size()) {
			sf::Vector2f targetPos = m_waypoints[m_currentWaypointIndex];

			// Distancia mínima para considerar que llegó al waypoint
			float distanceToTarget =
				std::sqrt(
					std::pow(targetPos.x - m_ACircle->getComponent<Transform>()->getPosition().x, 2) +
					std::pow(targetPos.y - m_ACircle->getComponent<Transform>()->getPosition().y, 2)
				);

			if (distanceToTarget < 10.0f) {
				// Si llegamos al waypoint actual, pasamos al siguiente
				m_currentWaypointIndex++;
			}
			else {
				// Movemos hacia el waypoint actual
				m_ACircle->getComponent<Transform>()
					->seek(targetPos, 100.f, m_windowPtr->deltaTime.asSeconds(), 10.f);
			}
		}
	}
}

void
BaseApp::render() {
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

void
BaseApp::destroy() {
}