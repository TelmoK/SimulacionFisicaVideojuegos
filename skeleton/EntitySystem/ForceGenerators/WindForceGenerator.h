#pragma once
#include "ForceGenerator.h"

#include "../../RenderItems/Particle.h"

class WindForceGenerator : public ForceGenerator
{
public:
	WindForceGenerator(EntitySystem* particle_system, Vector3D wind_velocity, float k1, float k2 = 0)
		: ForceGenerator(particle_system), wind_velocity(wind_velocity), k1(k1), k2(k2)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		Vector3D velocity_diff = (wind_velocity - particle->velocity());

		Vector3D force = velocity_diff * k1 + velocity_diff * velocity_diff.magnitude() * k2;

		particle->acceleration() += force / particle->mass();
	}

	/*
		Aplica la fuerza en un cuerpo que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(physx::PxRigidDynamic* body, double t) override
	{
		if (!_active) return;

		Vector3D velocity_diff = (wind_velocity - body->getLinearVelocity());

		Vector3D force = velocity_diff * k1 + velocity_diff * velocity_diff.magnitude() * k2;

		body->addForce(force.to_vec3());
	}

protected:

	Vector3D wind_velocity;
	float k1, k2;
};