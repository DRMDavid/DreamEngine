#pragma once
#pragma once

#include "../Prerequisites.h"

class CVector3 {
public:
	float x, y, z;

	CVector3() : x(0), y(0), z(0) {}
	CVector3(float x, float y, float z) : x(x), y(y), z(z) {}

	CVector3 
	operator+(const CVector3& other) const {
		return CVector3(x + other.x, y + other.y, z + other.z);
	}

	CVector3 
	operator-(const CVector3& other) const {
		return CVector3(x - other.x, y - other.y, z - other.z);
	}

	CVector3 
	operator*(float scalar) const {
		return CVector3(x * scalar, y * scalar, z * scalar);
	}

	CVector3
	operator/(float divisor) const {
		return CVector3(x / divisor, y / divisor, z / divisor);
	}

	CVector3& 
	operator+=(const CVector3& other) {
	x += other.x; y += other.y; z += other.z;
		return *this;
	}

	CVector3& 
	operator-=(const CVector3& other) {
	x -= other.x; y -= other.y; z -= other.z;
		return *this;
	}

	CVector3& 
	operator*=(float scalar) {
	x *= scalar; y *= scalar; z *= scalar;
		return *this;
	}

	CVector3& 
	operator/=(float scalar) {
	x /= scalar; y /= scalar; z /= scalar;
		return *this;
	}

	bool 
	operator==(const CVector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool 
	operator!=(const CVector3& other) const {
		return !(*this == other);
	}

	float& 
	operator[](int index) {
		return (index == 0) ? x : (index == 1) ? y : z;
	}

	const float& 
	operator[](int index) const {
		return (index == 0) ? x : (index == 1) ? y : z;
	}

	// Funciones geométricas
	float 
	lengthSquare() const {
		return x * x + y * y + z * z;
	}

	float 
	scaleProduct(const CVector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	CVector3 
	crossProduct(const CVector3& other) const {
		return CVector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	float 
	length() const {
		return std::sqrt(lengthSquare());
	}

	CVector3 
	normalized() const {
	float len = length();
		return (len == 0.f) ? CVector3(0.f, 0.f, 0.f) : (*this) / len;
	}

	void 
	normalize() {
		float len = length();
		if (len != 0.f) {
			x /= len; y /= len; z /= len;
		}
		else {
			x = y = z = 0.f;
		}
	}

	// Funciones estáticas
	inline static float 
	distance(const CVector3& a, const CVector3& b) {
		return (b - a).lengthSquare();
	}

	inline static CVector3 
	lerp(const CVector3& a, const CVector3& b, float t) {
		return a + (b - a) * t;
	}

	inline static CVector3
	zero() {
		return CVector3(0.f, 0.f, 0.f);
	}

	inline static CVector3 
	one() {
		return CVector3(1.f, 1.f, 1.f);
	}

	// Compatibilidad con SFML
	void 
	setPosition(const sf::Vector3f& position) {
		x = position.x; y = position.y; z = position.z;
	}

	void 
	move(const sf::Vector3f& offset) {
		x += offset.x; y += offset.y; z += offset.z;
	}

	void 
	setScale(const sf::Vector3f& scale) {
		x = scale.x; y = scale.y; z = scale.z;
	}

	void 
	scale(const sf::Vector3f& scale) {
		x *= scale.x; y *= scale.y; z *= scale.z;
	}

	void 
	setOrigin(const sf::Vector3f& origin) {
		x = origin.x; y = origin.y; z = origin.z;
	}
};
