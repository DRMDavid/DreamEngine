#pragma once

#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>

class window;

class Transform : public Component
{
public:
	// Constructor y destructor
	Transform() = default;
	virtual ~Transform() = default;

	// Métodos del ciclo de vida del componente
	void start() override;
	void update(float deltaTime) override;
	void render(const EngineUtilities::TSharedPointer<Window>& window) override;
	void destroy() override;

	// Setters
	void setPosition(const sf::Vector2f& position) { m_position = position; }
	void setRotation(const sf::Vector2f& rotation) { m_rotation = rotation; }
	void setScale(const sf::Vector2f& scale) { m_scale = scale; }

	// Getters
	void getPosition(sf::Vector2f& position) const { position = m_position; }
	void getRotation(sf::Vector2f& rotation) const { rotation = m_rotation; }
	void getScale(sf::Vector2f& scale) const { scale = m_scale; }

private:
	sf::Vector2f m_position;
	sf::Vector2f m_rotation;
	sf::Vector2f m_scale;
};
