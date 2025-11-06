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

	Vector3D& acceleration() {
		return _acceleration;
	}

	physx::PxTransform& transform() {
		return _transform;
	}

	physx::PxVec3& position() {
		return _transform.p;
	}

	physx::PxQuat& quat() {
		return _transform.q;
	}

	float& mass() {
		return _mass;
	}

	float& volume() {
		return _volume;
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
	float _volume = 1;
};