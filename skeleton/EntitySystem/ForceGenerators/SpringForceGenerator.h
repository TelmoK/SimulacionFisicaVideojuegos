#pragma once
#include "ForceGenerator.h"

#include "../../RenderItems/Particle.h"

class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(EntitySystem* particle_system, Particle* spring_origin_particle, float elastic_const, float resting_length)
		: ForceGenerator(particle_system), _spring_origin_particle(spring_origin_particle), _elastic_const(elastic_const), _resting_length(resting_length)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		Vector3 relative_pos = _spring_origin_particle->position() - particle->position();

		float length = relative_pos.normalize();
		float delta_x = length - _resting_length;

		Vector3D force = relative_pos * delta_x * _elastic_const;

		particle->acceleration() += force / particle->mass();
	}

protected:

	Particle* _spring_origin_particle = nullptr;
	float _elastic_const;
	float _resting_length;
};