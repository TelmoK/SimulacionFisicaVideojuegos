#include "Axis3D.h"

Axis3D::Axis3D(Vector3D position) : CompoundRenderItem()
{
	center_point_shape = CreateShape(physx::PxSphereGeometry(0.5));

	center_transform = new physx::PxTransform({position.x, position.y, position.z});

	Vector3D x_axis_vec = position + Vector3D(1, 0, 0) * _vector_arow_dist;
	x_axis_transform = new physx::PxTransform({ x_axis_vec.x, x_axis_vec.y, x_axis_vec.z });

	Vector3D y_axis_vec = position + Vector3D(0, 1, 0) * _vector_arow_dist;
	y_axis_transform = new physx::PxTransform({ y_axis_vec.x, y_axis_vec.y, y_axis_vec.z });

	Vector3D z_axis_vec = position + Vector3D(0, 0, 1) * _vector_arow_dist;
	z_axis_transform = new physx::PxTransform({ z_axis_vec.x, z_axis_vec.y, z_axis_vec.z });

	_sub_render_items.push_back(
		new RenderItem(center_point_shape, center_transform, Vector4(1, 1, 1, 1))
	);

	_sub_render_items.push_back(
		new RenderItem(center_point_shape, x_axis_transform, Vector4(1, 0, 0, 1))
	);

	_sub_render_items.push_back(
		new RenderItem(center_point_shape, y_axis_transform, Vector4(0, 1, 0, 1))
	);

	_sub_render_items.push_back(
		new RenderItem(center_point_shape, z_axis_transform, Vector4(0, 0, 1, 1))
	);
}

Axis3D::Axis3D() : Axis3D(Vector3D(0, 0, 0))
{

}

Axis3D::~Axis3D()
{
	//delete center_point_shape;
	delete center_transform;

	delete x_axis_transform;
	delete y_axis_transform;
	delete z_axis_transform;

	DeregisterCompoundRenderItem(this);
}