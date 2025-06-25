#pragma once

#include "ECS/Component.h"                  // Componente base del sistema ECS
#include <SFML/System/Vector2.hpp>         // Vector2f para representar posición, rotación y escala

// Declaración adelantada
class window;

// Componente Transform: maneja la transformación espacial de una entidad
class Transform : public Component {
public:
  // Constructor y destructor
  Transform() = default;
  virtual ~Transform() = default;

  // Métodos del ciclo de vida del componente
  void start() override;    // Inicialización
  void update(float deltaTime) override; // Lógica por frame
  void render(const EngineUtilities::TSharedPointer<Window>& window) override; // Render (opcional en Transform)
  void destroy() override;  // Limpieza

  // Setters
  inline void setPosition(const sf::Vector2f& pos) { m_position = pos; }
  inline void setRotation(const sf::Vector2f& rot) { m_rotation = rot; }
  inline void setScale(const sf::Vector2f& scl) { m_scale = scl; }

  // Getters
  inline void getPosition(sf::Vector2f& pos) const { pos = m_position; }
  inline void getRotation(sf::Vector2f& rot) const { rot = m_rotation; }
  inline void getScale(sf::Vector2f& scl) const { scl = m_scale; }

private:
  // Datos de transformación
  sf::Vector2f m_position;  // Posición 2D del objeto
  sf::Vector2f m_rotation;  // Rotación (vectorial o ángulo)
  sf::Vector2f m_scale;     // Escala (típicamente 1.0f, 1.0f)
};
