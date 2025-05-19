#pragma once
#include <prerequisites.h>
 
class
	BaseApp
{
public:
	BaseApp() = default;
	~BaseApp();

//funcion encargada de ejecutar en main
	int
	run();
//Funcion de inicializacion 
	bool
	init();

//funcion que actualiza por frame 
	void
	update();

//funcion de renderizado
	void 
	render();

	void 
	destroy();

	void 
	handleEvents();





private:

	sf::RenderWindow* m_window;
	sf::CircleShape* m_circle;
};