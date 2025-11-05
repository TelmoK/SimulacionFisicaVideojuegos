#pragma once
#include <array>
#include <cmath>

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

	Vector3 to_vec3() 
	{
		return Vector3(x, y, z);
	}
	
	float magnitude() const
	{
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
		return Vector3D(x, y, z) / this->magnitude();
	}
};