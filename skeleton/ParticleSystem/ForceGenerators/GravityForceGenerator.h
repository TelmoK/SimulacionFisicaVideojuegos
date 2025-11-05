#pragma once
#include "ForceGenerator.h"
#include "../../RenderItems/Particle.h"

class GravityForceGenerator : public ForceGenerator
{
public:
	GravityForceGenerator(ParticleSystem* particle_system, float gravityForce) 
		: ForceGenerator(particle_system), gravityForce(gravityForce){}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		particle->acceleration().y -= gravityForce;
	}

protected:

	float gravityForce;
};