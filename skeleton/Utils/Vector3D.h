#pragma once
#include <array>
#include <cmath>
#include <iostream>

#include "../core.hpp"

class Vector3D
{
public:

	float x, y, z;

	Vector3D(float x, float y, float z) : x(x), y(y), z(z){}
	Vector3D(Vector3 vec) : Vector3D(vec.x, vec.y, vec.z){}
	Vector3D() : Vector3D(0, 0, 0){}

	Vector3D operator+(Vector3D other) const
	{
		return Vector3D(x + other.x, y + other.y, z + other.z);
	}

	Vector3D operator-(Vector3D other) const
	{
		return Vector3D(x - other.x, y - other.y, z - other.z);
	}

	Vector3D operator*(float scalar) const
	{
		return Vector3D(x * scalar, y * scalar, z * scalar);
	}

	Vector3D operator/(float scalar) const
	{
		return Vector3D(x / scalar, y / scalar, z / scalar);
	}

	Vector3D operator=(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	void operator+=(const Vector3D& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator-=(const Vector3D& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	Vector3D operator-() const
	{
		return Vector3D(-x, -y, -z);
	}

	std::string to_str()
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	std::ostream& operator<<(std::ostream& out)
	{
		out << to_str();
		return out;
	}

	Vector3 to_vec3() const
	{
		return Vector3(x, y, z);
	}
	
	float magnitude() const
	{
		if(x == 0 && y == 0 && z == 0)
			return 0;

		return sqrt(x * x + y * y + z * z);
	}

	float dot(const Vector3D& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3D cross(const Vector3D& other) const
	{
		return Vector3D(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	Vector3D normalized() const
	{
		if(this->magnitude() == 0)
			return Vector3D(0, 0, 0);

		return Vector3D(x, y, z) / this->magnitude();
	}
};