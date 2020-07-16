#pragma once

#include "spdlog/fmt/ostr.h"

namespace molecool {
    
    struct  Vector
    {
        Vector();
        Vector(double x, double y, double z);

        double x, y, z;

		Vector operator+(const Vector& rhs) const;
		Vector& operator+=(const Vector& rhs);
		Vector operator-(const Vector& rhs) const;
		Vector& operator-=(const Vector& rhs);
		Vector& operator*=(double d);
		Vector operator/(double d) const;
		Vector& operator/=(double d);

		template<typename OStream>
		friend OStream& operator<<(OStream& os, const Vector& v)
		{
			return os << std::setprecision(6) << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		}

    };

	using Position = Vector;
	using Velocity = Vector;
	using Acceleration = Vector;
	using Force = Vector;

	Vector operator*(double d, Vector v);
	Vector operator*(Vector v, double d);

}




