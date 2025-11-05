#include "Particle.h"

Particle::Particle(Vector3D position, Vector3D velocity, float gravity, Vector3D acceleration)
	: RenderItem(CreateShape(physx::PxSphereGeometry(0.5)), &_transform, Vector4(1, 1, 1, 1)), _velocity(velocity)
{
	_transform = physx::PxTransform(position.to_vec3());

	_real_gravity = gravity;
	_gravity = _real_gravity;
	//_acceleration = acceleration + Vector3D(0, _gravity, 0);

	_real_velocity_factor = 3;
}

Particle::~Particle()
{
	DeregisterRenderItem(this);
}

Particle* Particle::dynamic_copy()
{
	return new Particle(_transform.p, _velocity, _gravity, _acceleration);
}

void Particle::integrate(double t)
{
	Vector3D new_position = Vector3D(_transform.p) + _velocity * t;
	_transform.p = new_position.to_vec3();

	_velocity = _velocity + _acceleration * t;

	//_acceleration.y = _gravity;

	simulateMass();
	simulateGravity();

	_acceleration = Vector3D(); // Reseteamos la aceleración
}

void Particle::simulateMass()
{
	_mass = _real_mass * pow(_real_velocity_factor, 2);
}

void Particle::simulateGravity()
{
	_gravity = _real_gravity * pow(_real_velocity_factor, -2);
}
