#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class Axis3D : public CompoundRenderItem
{
public:
	Axis3D();
	Axis3D(Vector3D position);

private:

	physx::PxShape* center_point_shape;
	physx::PxTransform* center_transform;

	physx::PxTransform* x_axis_transform;
	physx::PxTransform* y_axis_transform;
	physx::PxTransform* z_axis_transform;

	float _vector_arow_dist = 4; // Magnitude of the axis vector arows
};