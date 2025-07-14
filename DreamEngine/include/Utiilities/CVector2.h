#pragma once

#include "../Prerequisites.h"
#include <iostream>
#include <cmath>

/**
 * @class CVector2
 * @brief Clase que representa un vector 2D con operaciones basicas.
 *
 * Proporciona operaciones aritmeticas, comparacion,
 * acceso a elementos, normalizacion y funciones estaticas utiles.
 */
class CVector2 {
public:
  float x, y; ///< Componentes del vector

  /// Constructor por defecto, inicializa en (0,0)
  CVector2() : x(0), y(0) {}

  /// Constructor con valores explicitos
  CVector2(float x, float y) : x(x), y(y) {}

  /// Suma de vectores
  CVector2 operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
  }

  /// Resta de vectores
  CVector2 operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
  }

  /// Multiplicacion por escalar
  CVector2 operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
  }

  /// Division por escalar
  CVector2 operator/(float scalar) const {
    return CVector2(x / scalar, y / scalar);
  }

  /// Suma y asignacion
  CVector2& operator+=(const CVector2& other) {
    x += other.x; y += other.y;
    return *this;
  }

  /// Resta y asignacion
  CVector2& operator-=(const CVector2& other) {
    x -= other.x; y -= other.y;
    return *this;
  }

  /// Multiplicacion por escalar y asignacion
  CVector2& operator*=(float scalar) {
    x *= scalar; y *= scalar;
    return *this;
  }

  /// Division por escalar y asignacion
  CVector2& operator/=(float scalar) {
    x /= scalar; y /= scalar;
    return *this;
  }

  /// Comparacion de igualdad
  bool operator==(const CVector2& other) const {
    return x == other.x && y == other.y;
  }

  /// Comparacion de desigualdad
  bool operator!=(const CVector2& other) const {
    return !(*this == other);
  }

  /// Acceso a componente por indice (0=x, 1=y)
  float& operator[](int index) {
    return (index == 0) ? x : y;
  }

  /// Acceso a componente const por indice (0=x, 1=y)
  const float& operator[](int index) const {
    return (index == 0) ? x : y;
  }

  /// Cuadrado de la longitud del vector
  float lengthSquare() const {
    return x * x + y * y;
  }

  /// Longitud del vector (magnitud)
  float length() const {
    return std::sqrt(lengthSquare());
  }

  /// Producto punto con otro vector
  float dot(const CVector2& other) const {
    return x * other.x + y * other.y;
  }

  /// Vector normalizado sin modificar el original
  CVector2 normalized() const {
    float len = length();
    return (len == 0.f) ? CVector2(0.f, 0.f) : (*this) / len;
  }

  /// Normaliza el vector en sitio
  void normalize() {
    float len = length();
    if (len != 0.f) {
      x /= len; y /= len;
    }
    else {
      x = y = 0.f;
    }
  }

  /// Distancia al cuadrado entre dos vectores
  static float distance(const CVector2& a, const CVector2& b) {
    return (b - a).lengthSquare();
  }

  /// Interpolacion lineal entre dos vectores
  static CVector2 lerp(const CVector2& a, const CVector2& b, float t) {
    return a + (b - a) * t;
  }

  /// Vector (0,0)
  static CVector2 zero() {
    return CVector2(0.f, 0.f);
  }

  /// Vector (1,1)
  static CVector2 one() {
    return CVector2(1.f, 1.f);
  }

  /// Operador de salida para streams (ej. cout)
  friend std::ostream& operator<<(std::ostream& os, const CVector2& v) {
    os << "CVector2(" << v.x << ", " << v.y << ")";
    return os;
  }
};
