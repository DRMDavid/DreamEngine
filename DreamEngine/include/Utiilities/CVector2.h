/**
 * @file
 * @brief Clase para representar y operar con vectores 2D.
 * @details Implementa operaciones aritméticas, normalización, productos
 *  escalar y cruzado, así como utilidades estáticas comunes para el manejo
 *  de vectores bidimensionales.
 */

#pragma once
#include "../Prerequisites.h"
#include <iostream>
#include <cmath>

 /**
  * @class CVector2
  * @brief Clase para operaciones con vectores en 2D.
  * @details Permite realizar operaciones aritméticas, de normalización,
  *  productos y utilidades como interpolación lineal y cálculo de distancia.
  */
class CVector2 {
public:
  float x; ///< Componente X del vector.
  float y; ///< Componente Y del vector.

  /** @brief Constructor por defecto (vector (0,0)). */
  CVector2() : x(0), y(0) {}

  /**
   * @brief Constructor con valores iniciales.
   * @param X Componente X.
   * @param Y Componente Y.
   */
  CVector2(float X, float Y) : x(X), y(Y) {}

  /** @brief Suma de vectores. */
  CVector2 operator+(const CVector2& o) const { return CVector2(x + o.x, y + o.y); }

  /** @brief Resta de vectores. */
  CVector2 operator-(const CVector2& o) const { return CVector2(x - o.x, y - o.y); }

  /** @brief Multiplicación por escalar. */
  CVector2 operator*(float s) const { return CVector2(x * s, y * s); }

  /** @brief División por escalar. */
  CVector2 operator/(float s) const { return CVector2(x / s, y / s); }

  /** @brief Negación del vector. */
  CVector2 operator-() const { return CVector2(-x, -y); }

  /** @brief Suma y asignación. */
  CVector2& operator+=(const CVector2& o) { x += o.x; y += o.y; return *this; }

  /** @brief Resta y asignación. */
  CVector2& operator-=(const CVector2& o) { x -= o.x; y -= o.y; return *this; }

  /** @brief Multiplicación por escalar y asignación. */
  CVector2& operator*=(float s) { x *= s; y *= s; return *this; }

  /** @brief División por escalar y asignación. */
  CVector2& operator/=(float s) { x /= s; y /= s; return *this; }

  /** @brief Comparación de igualdad. */
  bool operator==(const CVector2& o) const { return x == o.x && y == o.y; }

  /** @brief Comparación de desigualdad. */
  bool operator!=(const CVector2& o) const { return !(*this == o); }

  /**
   * @brief Longitud al cuadrado del vector.
   * @return Magnitud al cuadrado.
   */
  float lengthSq() const { return x * x + y * y; }

  /**
   * @brief Longitud (magnitud) del vector.
   * @return Magnitud.
   */
  float length() const { return std::sqrt(lengthSq()); }

  /**
   * @brief Devuelve una copia normalizada del vector.
   * @return Vector unitario.
   */
  CVector2 normalized() const {
    float len = length();
    return len != 0 ? CVector2(x / len, y / len) : CVector2(0, 0);
  }

  /** @brief Normaliza el vector en sí mismo. */
  void normalize() {
    float len = length();
    if (len != 0) {
      x /= len;
      y /= len;
    }
  }

  /**
   * @brief Producto escalar con otro vector.
   * @param o Vector con el que multiplicar.
   * @return Resultado del producto punto.
   */
  float dot(const CVector2& o) const { return x * o.x + y * o.y; }

  /**
   * @brief Producto cruzado en 2D.
   * @param o Vector con el que multiplicar.
   * @return Escalar resultado del producto cruzado.
   */
  float cross(const CVector2& o) const { return x * o.y - y * o.x; }

  /** @brief Vector (0,0). */
  static CVector2 zero() { return CVector2(0, 0); }

  /** @brief Vector (1,1). */
  static CVector2 one() { return CVector2(1, 1); }

  /**
   * @brief Distancia entre dos vectores.
   * @param a Vector inicial.
   * @param b Vector final.
   * @return Distancia euclidiana.
   */
  static float distance(const CVector2& a, const CVector2& b) {
    return (a - b).length();
  }

  /**
   * @brief Interpolación lineal entre dos vectores.
   * @param a Vector inicial.
   * @param b Vector final.
   * @param t Factor de interpolación [0,1].
   * @return Vector interpolado.
   */
  static CVector2 lerp(const CVector2& a, const CVector2& b, float t) {
    return a + (b - a) * t;
  }
};

/**
 * @brief Operador de salida en flujo para CVector2.
 * @param os Flujo de salida.
 * @param v Vector a imprimir.
 * @return Referencia al flujo de salida.
 */
inline std::ostream& operator<<(std::ostream& os, const CVector2& v) {
  return os << "(" << v.x << ", " << v.y << ")";
}
