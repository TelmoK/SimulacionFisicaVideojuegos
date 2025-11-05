#pragma once
#include "Vector3D.h"

class SpaceGeometry
{
public:

	static Vector3D distanceLineToPoint(const Vector3D& line_point, const Vector3D& direction_vec, const Vector3D& point);

	static float cosineBetween(const Vector3D& vector_a, const Vector3D& vector_b);

	static float sineBetween(const Vector3D& vector_a, const Vector3D& vector_b);

	static float angleBetween(const Vector3D& vector_a, const Vector3D& vector_b);
};