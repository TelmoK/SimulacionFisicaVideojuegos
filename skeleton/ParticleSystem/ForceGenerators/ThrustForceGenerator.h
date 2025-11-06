#pragma once
#include "WindForceGenerator.h"
#include "../../RenderItems/Particle.h"

class ThrustForceGenerator : public ForceGenerator
{
public:
	ThrustForceGenerator(ParticleSystem* particle_system, float fluid_density, float gravity)
		: ForceGenerator(particle_system), _fluid_density(fluid_density), _gravity(gravity)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		Vector3D g = Vector3D(0, _gravity, 0);

		Vector3D force = -g * _fluid_density * particle->volume();

		particle->acceleration() += force / particle->mass();
	}

protected:

	float _fluid_density;
	float _gravity;
};