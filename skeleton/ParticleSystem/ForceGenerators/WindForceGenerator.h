#pragma once
#include "ForceGenerator.h"

#include "../../RenderItems/Particle.h"

class WindForceGenerator : public ForceGenerator
{
public:
	WindForceGenerator(ParticleSystem* particle_system, Vector3D wind_velocity, float k1, float k2 = 0)
		: ForceGenerator(particle_system), wind_velocity(wind_velocity), k1(k1), k2(k2)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		/*particle->acceleration().y -= gravityForce;*/
		Vector3D velocity_diff = (wind_velocity - particle->velocity());

		Vector3D force = velocity_diff * k1 + velocity_diff * velocity_diff.magnitude() * k2;

		particle->acceleration() += force;
	}

protected:

	Vector3D wind_velocity;
	float k1, k2;
};