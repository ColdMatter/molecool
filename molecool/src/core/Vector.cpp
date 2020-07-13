#include "mcpch.h"
#include "Vector.h"

namespace molecool {

	Vector::Vector()
		: x(0.0), y(0.0), z(0.0)
	{}

	Vector::Vector(double x, double y, double z)
		: x(x), y(y), z(z)
	{}

	Vector Vector::operator+(const Vector& rhs) const {
		return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector Vector::operator-(const Vector& rhs) const {
		return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector& Vector::operator+=(const Vector& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	// subtract another vector with result in original
	Vector& Vector::operator-=(const Vector& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	// multiply by by a scalar
	// note: works only for the scalar being on the right-hand side, i.e. V1 = V2 * d
	// to allow for left-hand scalar positions, must define the full function outside the class
	Vector& Vector::operator*=(double d) {
		this->x *= d;
		this->y *= d;
		this->z *= d;
		return *this;
	}

	// divide by a scalar
	Vector& Vector::operator/=(double d) {
		this->x /= d;
		this->y /= d;
		this->z /= d;
		return *this;
	}

	Vector Vector::operator/(double d) const {
		return Vector(x / d, y / d, z / d);
	}

	// define multiplication for scalar on the right of the vector, i.e. vec * d
	Vector operator*(Vector v, double d) {
		return v *= d;
	}

	// define multiplication for scalar on the left of the vector, i.e. d * vec
	Vector operator*(double d, Vector v) {
		return v *= d;
	}

}
