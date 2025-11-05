#pragma once
#include "ForceGenerator.h"

#include "../../RenderItems/Particle.h"

class ExplosiveForceGenerator : public ForceGenerator
{
public:
	ExplosiveForceGenerator(ParticleSystem* particle_system, float explosion_intensity, float explosion_radius)
		: ForceGenerator(particle_system), explosion_intensity(explosion_intensity), explosion_radius(explosion_radius)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		/*particle->acceleration().y -= gravityForce;
		Vector3D force = (explosion_intensity / 2) */
	}

protected:

	float explosion_intensity;
	float explosion_radius;
};