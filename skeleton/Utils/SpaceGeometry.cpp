#include "SpaceGeometry.h"

Vector3D SpaceGeometry::distanceLineToPoint(const Vector3D& line_point, const Vector3D& direction_vec, const Vector3D& point)
{
    if (direction_vec.magnitude() == 0) return Vector3D();

    Vector3D linepoint_to_point_vec = point - line_point;
    Vector3D proyection_in_line_vec = direction_vec.normalized() * (linepoint_to_point_vec.dot(direction_vec) / direction_vec.magnitude());

    return linepoint_to_point_vec - proyection_in_line_vec;
}

Vector3D SpaceGeometry::projectionVector(const Vector3D& vec_a, const Vector3D& vec_b)
{
    if (vec_b.magnitude() == 0) return Vector3D();

    return vec_b * (vec_a.dot(vec_b) / pow(vec_b.magnitude(), 2));
}

float SpaceGeometry::cosineBetween(const Vector3D& vector_a, const Vector3D& vector_b)
{
    if (vector_b.magnitude() == 0 || vector_a.magnitude() == 0) return 0;

    return (vector_a.dot(vector_b)) / (vector_a.magnitude() * vector_b.magnitude());
}

float SpaceGeometry::sineBetween(const Vector3D& vector_a, const Vector3D& vector_b)
{
    if (vector_b.magnitude() == 0 || vector_a.magnitude() == 0) return 0;

    return (vector_a.cross(vector_b).magnitude()) / (vector_a.magnitude() * vector_b.magnitude());
}

float SpaceGeometry::angleBetween(const Vector3D& vector_a, const Vector3D& vector_b)
{
    return acos(physx::PxClamp(SpaceGeometry::cosineBetween(vector_a, vector_b), -1.0f, 1.0f));
}
