#pragma once
#include "ForceGenerator.h"
#include "../../RenderItems/Particle.h"

class FrictionForceGenerator : public ForceGenerator
{
public:
	FrictionForceGenerator(EntitySystem* particle_system, float friction_factor)
		: ForceGenerator(particle_system), _friction_factor(friction_factor) {
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		particle->acceleration() += - particle->velocity() * _friction_factor;
	}

	void applyForce(physx::PxRigidDynamic* body, double t) override
	{
		if (!_active) return;

		body->addForce(-body->getLinearVelocity() * body->getMass() * _friction_factor);
	}

protected:

	// Entre 1 y 0
	float _friction_factor;
};