#pragma once

#include "ECS/Component.h"
#include <SFML/System/Vector2.hpp>

class
	window;

class
	Transform : public Component {

public:
	Transform() :
		m_position(0.f, 0.f),
		m_rotation(0.f, 0.f),
		m_scale(1.f, 1.f),
		Component(ComponentType::TRANSFORM) {
	}

	virtual
		~Transform() = default;
	//metodos de la clase Component
	void
		start() override;

	void
		update(float deltaTime)override;

	void
		render(const EngineUtilities::TSharedPointer<Window>& window)override;

	void
		destroy() override;

	//settters
	void
		setPosition(const sf::Vector2f& _position) {
		m_position = _position;
	}

	void
		setRotation(const sf::Vector2f& _rotation) {
		m_rotation = _rotation;
	}

	void
		setScale(const sf::Vector2f& _scale) {
		m_scale = _scale;
	}

	sf::Vector2f&
		getPosition() {
		return m_position;
	}


	sf::Vector2f&
		getRotation() {
		return m_rotation;
	}

	sf::Vector2f&
		getScale() {
		return m_scale;
	}
private:
	sf::Vector2f m_position;
	sf::Vector2f m_rotation;
	sf::Vector2f m_scale;
};