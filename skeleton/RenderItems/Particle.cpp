#include "Particle.h"

Particle::Particle(Vector3D position, Vector3D velocity) 
	: RenderItem(CreateShape(physx::PxSphereGeometry(0.5)), &_transform, Vector4(1, 1, 1, 1)), _velocity(velocity)
{
	_transform = physx::PxTransform(position.to_vec3());
	_acceleration = Vector3D(0,0,0);
}

Particle::~Particle()
{
	DeregisterRenderItem(this);
}

void Particle::integrate(double t)
{
	Vector3D new_position = Vector3D(transform->p) + _velocity * t;
	_transform.p = new_position.to_vec3();

	_velocity = _velocity + _acceleration * t;
}
