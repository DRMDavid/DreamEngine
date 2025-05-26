// Estilo de codificación:
// - camelCase para variables y métodos
// - PascalCase para clases
// - Tabulación: 2 espacios
// - Líneas de 80 a 90 caracteres máximo
// - Prefijo g_ para variables globales
// - Las clases y estructuras se definen con la llave de apertura en la misma línea
// - Los métodos dentro de clases se declaran con salto de línea entre el tipo y el nombre
// - La apertura del cuerpo del método se realiza en una nueva línea

#include "CShape.h"
#include "Window.h"

sf::Shape*
Cshape::createShape(ShapeType shapeType)
{
	switch (shapeType)
	{
	case ShapeType::CIRCLE:
	{
		sf::CircleShape* circle = new sf::CircleShape(10.0f);
		circle->setFillColor(sf::Color::Green);
		m_shape = circle;
		return circle;
	}
	case ShapeType::RECTANGLE:
	{
		sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(100.0f, 50.0f));
		rectangle->setFillColor(sf::Color::Red);
		m_shape = rectangle;
		return rectangle;
	}
	case ShapeType::TRIANGLE:
	{
		sf::ConvexShape* triangle = new sf::ConvexShape(3);
		triangle->setPointCount(3);
		triangle->setPoint(0, sf::Vector2f(0.0f, 0.0f));
		triangle->setPoint(1, sf::Vector2f(50.0f, 100.0f));
		triangle->setPoint(2, sf::Vector2f(100.0f, 0.0f));
		triangle->setFillColor(sf::Color::Blue);
		m_shape = triangle;
		return triangle;
	}
	case ShapeType::POLYGON:
	{
		sf::ConvexShape* polygon = new sf::ConvexShape(5);
		polygon->setPoint(0, sf::Vector2f(0.0f, 0.0f));
		polygon->setPoint(1, sf::Vector2f(50.0f, 100.0f));
		polygon->setPoint(2, sf::Vector2f(100.0f, 50.0f));
		polygon->setPoint(3, sf::Vector2f(150.0f, 100.0f));
		polygon->setPoint(4, sf::Vector2f(200.0f, 50.0f));
		polygon->setFillColor(sf::Color::Yellow);
		m_shape = polygon;
		return polygon;
	}
	default:
		break;
	}
	return nullptr;
}

void
Cshape::setPosition(float x, float y)
{
	if (m_shape)
	{
		m_shape->setPosition(x, y);
	}
	else
	{
		ERROR("Cshape", "setPosition", "Shape is null");
	}
}

void
Cshape::setPosition(const sf::Vector2f& position)
{
	if (m_shape)
	{
		m_shape->setPosition(position);
	}
	else
	{
		ERROR("Cshape", "setPosition", "Shape is null");
	}
}

void
Cshape::setFillColor(const sf::Color& color)
{
	if (m_shape)
	{
		m_shape->setFillColor(color);
	}
	else
	{
		ERROR("Cshape", "setFillColor", "Shape is null");
	}
}

void
Cshape::setRotation(float angle)
{
	if (m_shape)
	{
		m_shape->setRotation(angle);
	}
	else
	{
		ERROR("Cshape", "setRotation", "Shape is null");
	}
}

void
Cshape::setScale(const sf::Vector2f& scl)
{
	if (m_shape)
	{
		m_shape->setScale(scl);
	}
	else
	{
		ERROR("Cshape", "setScale", "Shape is null");
	}
}
