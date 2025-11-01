#pragma once
#include "Vector2D.h"


struct Collision {
	enum Type {
		NONE,
		ENEMY,
		PLATFORM
	};

	Type collisionType = NONE;
	Vector2D<float> velocity{ 0, 0 };

	Collision() : collisionType(NONE), velocity(0, 0) {}

	Collision(Type tipo, const Vector2D<float>& velocidad)
		: collisionType(tipo), velocity(velocidad) {
	}
};