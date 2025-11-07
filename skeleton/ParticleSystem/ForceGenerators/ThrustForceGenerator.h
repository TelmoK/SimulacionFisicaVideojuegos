#pragma once
#include "WindForceGenerator.h"
#include "../../RenderItems/Particle.h"

class ThrustForceGenerator : public ForceGenerator
{
public:
	ThrustForceGenerator(
		ParticleSystem* particle_system, float fluid_density, float gravity,
		physx::PxBoxGeometry* fluid_area = nullptr, physx::PxTransform* fluid_area_transform = nullptr
	)
	:	ForceGenerator(particle_system), _fluid_density(fluid_density), _gravity(gravity), 
		_fluid_area(fluid_area), _fluid_area_transform(fluid_area_transform)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		if (_fluid_area == nullptr || _fluid_area_transform == nullptr) {
			applyForceInWorld(particle, t);
			return;
		}

		applyForceInArea(particle, t);
	}

protected:

	void applyForceInWorld(Particle* particle, double t);

	void applyForceInArea(Particle* particle, double t);


	float _fluid_density;
	float _gravity;

	// Por si el efecto se limita a una región del espacio como un mar
	physx::PxBoxGeometry* _fluid_area = nullptr;
	physx::PxTransform* _fluid_area_transform;
};