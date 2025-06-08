#pragma once

// ==============================
// Librerías estándar (STD)
// ==============================
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <unordered_map>

// ==============================
// Librerías de terceros
// ==============================
#include <SFML/Graphics.hpp>

// ==============================
// Macros
// ==============================

/**
 * @brief Libera de forma segura un puntero dinámico.
 */
#define SAFE_PTR_RELEASE(x) if(x != nullptr) { delete x; x = nullptr; }

 /**
  * @brief Imprime mensaje de creación de recurso.
  * @param classObj Nombre de la clase.
  * @param method   Método donde se creó el recurso.
  * @param state    Estado o tipo de recurso creado.
  */
#define MESSAGE(classObj, method, state)                        \
{                                                               \
  std::ostringstream os_;                                       \
  os_ << classObj << "::" << method << " : "                    \
      << "[CREATION OF RESOURCE : " << state << "]\n";          \
  std::cerr << os_.str();                                       \
}

  /**
   * @brief Imprime error y finaliza ejecución.
   * @param classObj Nombre de la clase.
   * @param method   Método donde ocurrió el error.
   * @param errorMSG Mensaje de error.
   */
#define ERROR(classObj, method, errorMSG)                       \
{                                                               \
  std::ostringstream os_;                                       \
  os_ << "ERROR : " << classObj << "::" << method << " : "      \
      << "Error in data from params [" << errorMSG << "]\n";    \
  std::cerr << os_.str();                                       \
  exit(1);                                                      \
}

   // ==============================
   // Enumeraciones
   // ==============================

   /**
    * @enum ShapeType
    * @brief Tipos de figura compatibles.
    */
enum
  ShapeType {
  EMPTY = 0, ///< No definido
  CIRCLE = 1, ///< Círculo
  RECTANGLE = 2, ///< Rectángulo
  TRIANGLE = 3, ///< Triángulo
  POLYGON = 4  ///< Polígono personalizado
};
