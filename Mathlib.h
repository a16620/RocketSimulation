#pragma once
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include <stdexcept>
#include <vector>
#include <iostream>

#ifndef __MATH__LIB__INCLUDED__
#define __MATH__LIB__INCLUDED__
#ifdef _MATHLIB_USE_DOUBLE
#define _m_d_float double
#define _m_sqrt sqrt
#define _m_pow pow
#else
#define _m_d_float float
#define _m_sqrt sqrtf
#define _m_pow powf
#endif

#ifdef _MATHLIB_USE_LONGINT
#define _m_d_int long int
#else
#define _m_d_int int
#endif

class Vector2 {
public:
	_m_d_float x;
	_m_d_float y;

	//Operators

	_m_d_float magnitude() const {
		return _m_sqrt(_m_pow(x, 2) + _m_pow(y, 2));
	}

	_m_d_float sqrtMagnitude() const {
		return _m_pow(x, 2) + _m_pow(y, 2);
	}

	Vector2 unit() const {
		Vector2 unit(*this);
		unit /= magnitude();
		return unit;
	}

	_m_d_float ScalarProduct(const Vector2& v) const {
		return x * v.x + y + v.y;
	}

	//Rotate PI/2
	Vector2 perpendicularPositive() const {
		return Vector2(-y, x);
	}

	//Rotate -PI/2
	Vector2 perpendicularNegative() const {
		return Vector2(y, -x);
	}

	Vector2& operator+=(const Vector2& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2& operator*=(_m_d_float k) {
		x *= k;
		y *= k;
		return *this;
	}

	Vector2& operator/=(_m_d_float k) {
		x /= k;
		y /= k;
		return *this;
	}

	const Vector2 operator+(const Vector2& vec) const {
		return Vector2(x + vec.x, y + vec.y);
	}

	const Vector2 operator-(const Vector2& vec) const {
		return Vector2(x - vec.x, y - vec.y);
	}

	const Vector2 operator*(float k) const {
		return Vector2(x*k, y*k);
	}

	const Vector2 operator/(float k) const {
		return Vector2(x / k, y / k);
	}

	Vector2() : x(0), y(0) {}
	Vector2(_m_d_float _x, _m_d_float _y) : x(_x), y(_y) {}
	Vector2(const Vector2& vector) : x(vector.x), y(vector.y) {}
};
typedef Vector2* PVector2;

#endif