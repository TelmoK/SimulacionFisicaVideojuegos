#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class Particle : public RenderItem
{
public:
	Particle(Vector3D position, Vector3D velocity);
	~Particle();

	void integrate(double t);

private:
	Vector3D _velocity;
	Vector3D _acceleration;
	physx::PxTransform _transform;
	float _mass = 1; // In kg
};