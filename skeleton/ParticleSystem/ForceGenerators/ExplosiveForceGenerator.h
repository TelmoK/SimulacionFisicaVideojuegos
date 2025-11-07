#pragma once
#include "ForceGenerator.h"

#include "../../RenderItems/Particle.h"
#include <cmath>

class ExplosiveForceGenerator : public ForceGenerator
{
public:
	ExplosiveForceGenerator(
		ParticleSystem* particle_system, Vector3D center, float explosion_intensity, 
		float explosion_radius, float explosion_duration
	)
		: ForceGenerator(particle_system), _explosion_center(center), _explosion_intensity(explosion_intensity),
		_explosion_radius(explosion_radius), _explosion_duration(explosion_duration)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		Vector3D particle_dist = (particle->position() - _explosion_center.to_vec3());

		Vector3D force = particle_dist * (_explosion_intensity / particle_dist.magnitude())
			* pow(std::exp(1.0), -t/ _time_const);

		if(particle_dist.magnitude() < _explosion_radius)
			particle->acceleration() += force / particle->mass();
	}

	void explode() {
		_time_const = _explosion_duration;
	}

	void update(float t) override
	{
		if(_time_const <= 0)
			_time_const -= t;
	}

protected:

	Vector3D _explosion_center;
	float _explosion_intensity;
	float _explosion_radius;
	float _explosion_duration;
	float _time_const;
};