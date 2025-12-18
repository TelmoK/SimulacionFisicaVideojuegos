#pragma once
#include "WindForceGenerator.h"
#include "../../RenderItems/Particle.h"
#include "../../RenderItems/RenderBody.h"

class ThrustForceGenerator : public ForceGenerator
{
public:
	ThrustForceGenerator(
		EntitySystem* particle_system, float fluid_density, float gravity, float objects_height,
		physx::PxBoxGeometry* fluid_area = nullptr, physx::PxTransform* fluid_area_transform = nullptr
	)
		: ForceGenerator(particle_system), _fluid_density(fluid_density), _gravity(gravity), 
		_fluid_area(fluid_area), _fluid_area_transform(fluid_area_transform), _objects_height(objects_height)
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de entidades.
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

	/*
		Aplica la fuerza en un cuerpo que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(RenderBody* renderBody, double t) override
	{
		if (!_active) return;

		if (_fluid_area == nullptr || _fluid_area_transform == nullptr) {
			applyForceInWorld(renderBody->body(), t);
			return;
		}

		applyForceInArea(renderBody->body(), t);
	}

protected:

	void applyForceInWorld(Particle* particle, double t);

	void applyForceInArea(Particle* particle, double t);

	void applyForceInWorld(physx::PxRigidDynamic* body, double t);

	void applyForceInArea(physx::PxRigidDynamic* body, double t);


	float _fluid_density;
	float _gravity;
	float _objects_height;

	// Por si el efecto se limita a una región del espacio como un mar
	physx::PxBoxGeometry* _fluid_area = nullptr;
	physx::PxTransform* _fluid_area_transform;
};