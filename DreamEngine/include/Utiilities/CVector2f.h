#pragma once

#include <sstream>
#include <iostream>

#include "Prerequisites.h" // macros ERROR, MESSAGE, etc.

namespace Utilities {

  class CVector2 {
  public:

    float x;
    float y;

    // Constructores
    CVector2() : x(0.f), y(0.f) {}
    CVector2(float x, float y) : x(x), y(y) {}
    CVector2(const CVector2& other) : x(other.x), y(other.y) {}

    // Operadores aritméticos
    CVector2 operator+(const CVector2& rhs) const {
      return CVector2(x + rhs.x, y + rhs.y);
    }

    CVector2 operator-(const CVector2& rhs) const {
      return CVector2(x - rhs.x, y - rhs.y);
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

    // Operadores compuestos
    CVector2& operator+=(const CVector2& rhs) {
      x += rhs.x;
      y += rhs.y;
      return *this;
    }

    CVector2& operator-=(const CVector2& rhs) {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
    }

    CVector2& operator*=(float scalar) {
      x *= scalar;
      y *= scalar;
      return *this;
    }

    CVector2& operator/=(float scalar) {
      if (scalar == 0.f) {
        ERROR("CVector2", "operator/=", "Division por cero");
        x = 0.f; y = 0.f;
      }
      else {
        x /= scalar;
        y /= scalar;
      }
      return *this;
    }

    // Comparaciones
    bool operator==(const CVector2& rhs) const {
      return (x == rhs.x) && (y == rhs.y);
    }

    bool operator!=(const CVector2& rhs) const {
      return !(*this == rhs);
    }

    // Acceso por índice
    float& operator[](int index) {
      if (index == 0) return x;
      else if (index == 1) return y;
      else {
        ERROR("CVector2", "operator[]", "Índice fuera de rango");
        return x; // Retorno seguro para evitar errores
      }
    }

    const float& operator[](int index) const {
      if (index == 0) return x;
      else if (index == 1) return y;
      else {
        ERROR("CVector2", "operator[] const", "Índice fuera de rango");
        return x;
      }
    }

    // Función auxiliar para raíz cuadrada con Newton-Raphson
    static float sqrtNewton(float number) {
      if (number <= 0.f) return 0.f;
      float x = number;
      for (int i = 0; i < 10; ++i) {
        x = 0.5f * (x + number / x);
      }
      return x;
    }

    // Funciones geométricas
    float length() const {
      return sqrtNewton(x * x + y * y);
    }

    float lengthSquared() const {
      return x * x + y * y;
    }

    float dot(const CVector2& rhs) const {
      return x * rhs.x + y * rhs.y;
    }

    float cross(const CVector2& rhs) const {
      return x * rhs.y - y * rhs.x;
    }

    // Normalización
    CVector2 normalized() const {
      float len = length();
      if (len == 0.f) {
        return CVector2(0.f, 0.f);
      }
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

    // Métodos estáticos de utilidad
    static float distance(const CVector2& a, const CVector2& b) {
      return (a - b).length();
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

    // Para depuración
    std::string toString() const {
      std::ostringstream oss;
      oss << "CVector2(" << x << ", " << y << ")";
      return oss.str();
    }

    // Métodos de transformación para simular sf::Vector2f
    void setPosition(const CVector2& position) {
      x = position.x;
      y = position.y;
    }

    void move(const CVector2& offset) {
      x += offset.x;
      y += offset.y;
    }

    void setScale(const CVector2& scale) {
      x = scale.x;
      y = scale.y;
    }

    void scale(const CVector2& factors) {
      x *= factors.x;
      y *= factors.y;
    }

    void setOrigin(const CVector2& origin) {
     
    }

  };

} // namespace Utilities
