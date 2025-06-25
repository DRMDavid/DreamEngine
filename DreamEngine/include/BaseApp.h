#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "CShape.h"
class 
BaseApp {
public:
	BaseApp() = default;
	~BaseApp();

	int
	run();

	bool
	init();

	void
	update();

	void
	render();

	void
	destroy();


private:
	EngineUtilities::TSharedPointer<Window> m_windowPtr;

	EngineUtilities::TSharedPointer<CShape>   m_shapePtr;
};