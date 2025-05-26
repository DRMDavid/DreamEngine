// Estilo de codificación:
// - camelCase para variables y métodos
// - PascalCase para clases
// - Tabulación: 2 espacios
// - Líneas de 80 a 90 caracteres máximo
// - Prefijo g_ para variables globales
// - Las clases y estructuras se definen con la llave de apertura en la misma línea
// - Los métodos dentro de clases se declaran con salto de línea entre el tipo y el nombre
// - La apertura del cuerpo del método se realiza en una nueva línea

#pragma once
#include "Prerequisites.h"

class
	Window {
public:
	Window() = default;
	Window(int width, int height, const std::string& title);
	~Window();

	void
		handleEvents();

	bool
		isOpen() const;

	void
		clear(const sf::Color& color = sf::Color(0, 0, 0, 225));

	void
		draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

	void
		display();

	void
		destroy();

private:
	sf::RenderWindow* m_window;
	sf::View m_view;
};
#pragma once
