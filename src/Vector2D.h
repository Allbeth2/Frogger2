#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

/**
 * Vector bidimensional genérico.
 */
template<typename T = float>
class Vector2D
{
	T x, y;

public:
	Vector2D(T x, T y) : x(x), y(y) {}
	Vector2D() : Vector2D(0, 0) {}

	// Coordenadas del vector
	const T& getX() const { return x; }
	const T& getY() const { return y; }

	// Operadores
	Vector2D operator+(const Vector2D& otro) const {
		return { x + otro.x, y + otro.y };
	}

	Vector2D operator-(const Vector2D& otro) const {
		return { x - otro.x, y - otro.y };
	}

	// Operador de igualdad (necesario para comparar posiciones)
	bool operator==(const Vector2D& otro) const {
		return x == otro.x && y == otro.y;
	}

	bool operator!=(const Vector2D& otro) const {
		return !(*this == otro);
	}

	// Producto escalar
	T operator*(const Vector2D& otro) const {
		return x * otro.x + y * otro.y;
	}

	// Producto por escalar
	Vector2D operator*(T escalar) const {
		return { x * escalar, y * escalar };
	}

	// Operadores de entrada/salida
	friend std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
		return out << '{' << v.x << ", " << v.y << '}';
	}
};

// Alias para puntos 2D
template<typename T>
using Point2D = Vector2D<T>;

#endif // VECTOR2D_H