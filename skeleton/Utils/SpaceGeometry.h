#pragma once
#include "Vector3D.h"

class SpaceGeometry
{
public:

	static Vector3D distanceLineToPoint(const Vector3D& line_point, const Vector3D& direction_vec, const Vector3D& point);

	/*
		Returns the projection vector of vec_a onto vec_b
	*/
	static Vector3D projectionVector(const Vector3D& vec_a, const Vector3D& vec_b);

	static float cosineBetween(const Vector3D& vector_a, const Vector3D& vector_b);

	static float sineBetween(const Vector3D& vector_a, const Vector3D& vector_b);

	static float angleBetween(const Vector3D& vector_a, const Vector3D& vector_b);
};