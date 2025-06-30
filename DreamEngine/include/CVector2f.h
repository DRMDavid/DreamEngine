#pragma once

#include "prerequisites.h"

class CVector2 {
public:
  float x;
  float y;

  // Constructores
  CVector2() : x(0.f), y(0.f) {}
  CVector2(float xVal, float yVal) : x(xVal), y(yVal) {}

  // Operadores
  CVector2 operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
  }

  CVector2 operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
  }

  CVector2 operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
  }

  CVector2 operator/(float scalar) const {
    if (scalar == 0.f) {
      ERROR("CVector2", "operator/", "Division por cero");
      return CVector2(0.f, 0.f);
    }
    return CVector2(x / scalar, y / scalar);
  }

  bool operator==(const CVector2& other) const {
    return (x == other.x) && (y == other.y);
  }

  float& operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    ERROR("CVector2", "operator[]", "Índice fuera de rango");
    return x;
  }

  const float& operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    ERROR("CVector2", "operator[] const", "Índice fuera de rango");
    return x;
  }

  // Funciones geométricas
  float lengthSquared() const {
    return x * x + y * y;
  }

  // Raíz cuadrada con Newton-Raphson
  static float sqrtNewton(float number) {
    if (number <= 0.f) return 0.f;
    float approx = number;
    for (int i = 0; i < 10; ++i) {
      approx = 0.5f * (approx + number / approx);
    }
    return approx;
  }

  float length() const {
    return sqrtNewton(lengthSquared());
  }

  float dot(const CVector2& other) const {
    return x * other.x + y * other.y;
  }

  float cross(const CVector2& other) const {
    return x * other.y - y * other.x;
  }

  CVector2 normalized() const {
    float len = length();
    if (len == 0.f) return CVector2(0.f, 0.f);
    return (*this) / len;
  }

  void normalize() {
    float len = length();
    if (len == 0.f) {
      x = 0.f;
      y = 0.f;
    }
    else {
      x /= len;
      y /= len;
    }
  }

  // Utilidades
  static float distance(const CVector2& a, const CVector2& b) {
    return (b - a).length();
  }

  static CVector2 lerp(const CVector2& a, const CVector2& b, float t) {
    if (t < 0.f) t = 0.f;
    else if (t > 1.f) t = 1.f;
    return a + (b - a) * t;
  }

  static CVector2 zero() {
    return CVector2(0.f, 0.f);
  }

  static CVector2 one() {
    return CVector2(1.f, 1.f);
  }

  // Debug
  void print() const {
    MESSAGE("CVector2", "print", ("(" + std::to_string(x) + ", " + std::to_string(y) + ")").c_str());
  }
};
