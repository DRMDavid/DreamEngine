// Estilo de codificación:
// - camelCase para variables y métodos
// - PascalCase para clases
// - Tabulación: 2 espacios
// - Líneas de 80 a 90 caracteres máximo
// - Prefijo g_ para variables globales

#pragma once
// STD libraries
#include <iostream>
#include <string>
#include <Sstream>
#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <unordered_map>


//third party libraries
#include <SFML/Graphics.hpp>


// MACRO for safe release of resources
#define SAFE_PTR_RELEASE(x) if(x != nullptr) { delete x; x = nullptr; }

#define MESSAGE(classObj, method, state)                      \
{                                                             \
    std::ostringstream os_;                                   \
    os_ << classObj << "::" << method << " : "                \
        << "[CREATION OF RESOURCE" << ": " << state "] \n";\
    std::cerr << os_.str();                                   \
}

#define ERROR(classObj, method, errorMSG)                         \
{                                                                 \
    std::ostringstream os_;                                       \
    os_ << "ERROR : " << classObj << "::" << method << " : "      \
        << "  Error in data from params [" << errorMSG"] \n"; \
    std::cerr << os_.str();                                       \
    exit(1);                                                      \
}

//enums
enum
    ShapeType
{
    EMPTY = 0,
    CIRCLE = 1,
    RECTANGLE = 2,
    TRIANGLE = 3,
    POLYGON = 4
};