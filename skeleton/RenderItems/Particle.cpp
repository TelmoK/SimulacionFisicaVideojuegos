#include "Particle.h"

#include "../ParticleSystem/ParticleSystem.h"

Particle::Particle(ParticleSystem* p_sys, Vector3D position, Vector3D velocity, Vector3D acceleration, float mass, float volume)
	: Particle(position, velocity, acceleration, mass, volume)
{
		p_sys->registerNewParticle(this, 5);
}

Particle::Particle(Vector3D position, Vector3D velocity, Vector3D acceleration, float mass, float volume)
	: RenderItem(CreateShape(physx::PxSphereGeometry(0.5)), &_transform, Vector4(1, 1, 1, 1)), 
	_velocity(velocity), _acceleration(acceleration), _real_mass(mass), _volume(volume)
{
	_transform = physx::PxTransform(position.to_vec3());

	_real_velocity_factor = 3;
}

Particle::~Particle()
{
	DeregisterRenderItem(this);
}

Particle* Particle::dynamic_copy()
{
	return new Particle(_transform.p, _velocity, _acceleration, _real_mass);
}

void Particle::integrate(double t)
{
	Vector3D new_position = Vector3D(_transform.p) + _velocity * t;
	_transform.p = new_position.to_vec3();

	_velocity = _velocity + _acceleration * t;

	simulateMass();
	// simulateGravity();

	_acceleration = Vector3D(); // Reseteamos la aceleración
}

void Particle::simulateMass()
{
	_mass = _real_mass * pow(_real_velocity_factor, 2);
}

/*
void Particle::simulateGravity()
{
	_gravity = _real_gravity * pow(_real_velocity_factor, -2);
}*/
