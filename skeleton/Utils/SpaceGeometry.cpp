#include "SpaceGeometry.h"

Vector3D SpaceGeometry::distanceLineToPoint(const Vector3D& line_point, const Vector3D& direction_vec, const Vector3D& point)
{
    Vector3D linepoint_to_point_vec = point - line_point;
    Vector3D proyection_in_line_vec = direction_vec.normalized() * (linepoint_to_point_vec.dot(direction_vec) / direction_vec.magnitude());

    return linepoint_to_point_vec - proyection_in_line_vec;
}

float SpaceGeometry::cosineBetween(const Vector3D& vector_a, const Vector3D& vector_b)
{
    return (vector_a.dot(vector_b)) / (vector_a.magnitude() * vector_b.magnitude());
}

float SpaceGeometry::sineBetween(const Vector3D& vector_a, const Vector3D& vector_b)
{
    return (vector_a.cross(vector_b).magnitude()) / (vector_a.magnitude() * vector_b.magnitude());
}

float SpaceGeometry::angleBetween(const Vector3D& vector_a, const Vector3D& vector_b)
{
    return acos(physx::PxClamp(SpaceGeometry::cosineBetween(vector_a, vector_b), -1.0f, 1.0f));
}
