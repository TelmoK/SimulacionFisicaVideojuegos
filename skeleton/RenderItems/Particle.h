#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class Particle : public RenderItem
{
public:
	Particle(Vector3D position, Vector3D velocity, float gravity = 0, Vector3D acceleration = Vector3D());
	~Particle();

	void integrate(double t);

	Particle* dynamic_copy();

	Vector3D& velocity() {
		return _velocity;
	}

	physx::PxTransform& transform() {
		return _transform;
	}

protected:
	void simulateMass();
	void simulateGravity();

private:
	physx::PxTransform _transform;

	Vector3D _velocity;
	float _real_velocity_factor; // (real_velocity / simulated_velocity)

	Vector3D _acceleration;
	float _real_gravity;
	float _gravity;

	float _mass = 1; // In kg
	float _real_mass = 1;
};