#pragma once

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

    };

	using Position = Vector;
	using Velocity = Vector;
	using Acceleration = Vector;
	using Force = Vector;

}


