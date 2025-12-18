#pragma once
#include "ForceGenerator.h"
#include "../../RenderItems/Particle.h"

class FrictionForceGenerator : public ForceGenerator
{
public:
	FrictionForceGenerator(
		EntitySystem* particle_system, float friction_factor, 
		physx::PxBoxGeometry* fluid_area = nullptr, physx::PxTransform* fluid_area_transform = nullptr
	)
		: ForceGenerator(particle_system), _friction_factor(friction_factor) {
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		// Si hay un área de actuación solo aplicamos la resistencia si el objeto está sumergido
		if (_fluid_area != nullptr && _fluid_area_transform != nullptr) {
			float radius = pow(3 * particle->volume() / 4 * physx::PxPi, 0.3333);
			float particle_bottom = particle->transform().p.y - radius;
			float fluid_height = _fluid_area_transform->p.y + _fluid_area->halfExtents.y;

			if (fluid_height < particle_bottom) return;
		}

		particle->acceleration() += - particle->velocity() * _friction_factor;
	}

	void applyForce(RenderBody* renderBody, double t) override
	{
		if (!_active) return;

		physx::PxRigidDynamic* body = renderBody->body();

		// Si hay un área de actuación solo aplicamos la resistencia si el objeto está sumergido
		if (_fluid_area != nullptr && _fluid_area_transform != nullptr) {
			float body_bottom = body->getGlobalPose().p.y - body->getWorldBounds().getExtents().y;
			float fluid_height = _fluid_area_transform->p.y + _fluid_area->halfExtents.y;

			if (fluid_height < body_bottom) return;
		}

		body->addForce(-body->getLinearVelocity() * body->getMass() * _friction_factor);
	}

protected:

	// Entre 1 y 0
	float _friction_factor;

	// Por si el efecto se limita a una región del espacio como un mar
	physx::PxBoxGeometry* _fluid_area = nullptr;
	physx::PxTransform* _fluid_area_transform;
};