/**
 * @file
 * @brief Archivo de configuración y utilidades generales (Prerequisites).
 * @details Contiene definiciones comunes, macros, includes compartidos y
 *  enumeraciones base usadas en el motor, como tipos de formas y macros
 *  para manejo seguro de punteros y mensajes de depuración.
 */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "Memory/TSharedPointer.h"
#include "Memory/TWeakPointer.h"
#include "Memory/TStaticPtr.h"
#include "Memory/TUniquePtr.h"

#include <imgui.h>
#include <imgui-SFML.h>

 /**
	* @brief Libera un puntero y lo pone en `nullptr` si no es nulo.
	* @param x Puntero a liberar.
	*/
#define SAFE_PTR_RELEASE(x) \
	if (x != nullptr) { delete x; x = nullptr; }

	/**
	 * @brief Macro para mostrar un mensaje de creación de recurso.
	 * @param classObj Nombre de la clase.
	 * @param method Nombre del método.
	 * @param state Estado o detalle de la creación.
	 */
#define MESSAGE(classObj, method, state)                        \
{                                                               \
	std::ostringstream os_;                                     \
	os_ << classObj << "::" << method << " : "                  \
		<< "[CREATION OF RESOURCE" << ": " << state << "] \n";  \
	std::cerr << os_.str();                                     \
}

	 /**
		* @brief Macro para mostrar un mensaje de error y finalizar la ejecución.
		* @param classObj Nombre de la clase.
		* @param method Nombre del método.
		* @param errorMSG Mensaje de error específico.
		*/
#define ERROR(classObj, method, errorMSG)                          \
{                                                                  \
	std::ostringstream os_;                                        \
	os_ << "ERROR : " << classObj << "::" << method << " : "       \
		<< "  Error in data from params [" << errorMSG << "] \n";  \
	std::cerr << os_.str();                                        \
	exit(1);                                                       \
}

		/**
		 * @enum ShapeType
		 * @brief Tipos de forma soportados por el motor.
		 */
enum ShapeType {
	EMPTY = 0,    ///< Sin forma.
	CIRCLE = 1,   ///< Círculo.
	RECTANGLE = 2,///< Rectángulo.
	TRIANGLE = 3, ///< Triángulo.
	POLYGON = 4   ///< Polígono.
};
