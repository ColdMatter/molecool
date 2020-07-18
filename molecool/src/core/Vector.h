#pragma once

namespace molecool {
    
    struct  Vector
    {
        explicit Vector(double* xPtr);
        Vector(double x = 0.0, double y = 0.0, double z = 0.0);

        double x, y, z;

		Vector operator+(const Vector& rhs) const;
		Vector& operator+=(const Vector& rhs);
		Vector operator-(const Vector& rhs) const;
		Vector& operator-=(const Vector& rhs);
		Vector& operator*=(double d);
		Vector operator/(double d) const;
		Vector& operator/=(double d);

		friend std::ostream& operator<<(std::ostream& os, const Vector& v);

    };

	using Position = Vector;
	using Velocity = Vector;
	using Acceleration = Vector;
	using Force = Vector;

	Vector operator*(double d, Vector v);
	Vector operator*(Vector v, double d);

}




