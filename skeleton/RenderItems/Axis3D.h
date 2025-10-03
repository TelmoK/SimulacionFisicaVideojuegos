#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class Axis3D : public CompoundRenderItem
{
public:
	Axis3D();
	Axis3D(Vector3D position);
	~Axis3D();

private:

	physx::PxShape* center_point_shape = nullptr;
	physx::PxTransform* center_transform = nullptr;

	physx::PxTransform* x_axis_transform = nullptr;
	physx::PxTransform* y_axis_transform = nullptr;
	physx::PxTransform* z_axis_transform = nullptr;

	float _vector_arow_dist = 4; // Magnitude of the axis vector arows
};