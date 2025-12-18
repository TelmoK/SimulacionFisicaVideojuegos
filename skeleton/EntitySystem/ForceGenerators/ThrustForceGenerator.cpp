#include "ThrustForceGenerator.h"

#include <foundation/PxMat33.h>
#include <geometry/PxGeometryQuery.h>
#include <PxQueryReport.h>

#include <iostream>

void ThrustForceGenerator::applyForceInWorld(Particle* particle, double t)
{
	Vector3D g = Vector3D(0, _gravity, 0);

	Vector3D force = -g * _fluid_density * particle->volume();

	particle->acceleration() += force / particle->mass();
}

void ThrustForceGenerator::applyForceInArea(Particle* particle, double t)
{
	float h = particle->transform().p.y;
	float h0 = _fluid_area_transform->p.y + _fluid_area->halfExtents.y;

	float immersed = 0;
	if (h - h0 > _objects_height * 0.5)
		immersed = 0;
	else if (h0 - h > _objects_height * 0.5)
		immersed = 1;
	else
		immersed = (h0 - h) / _objects_height + 0.5;

	Vector3D g = Vector3D(0, _gravity, 0);

	Vector3D force = -g * _fluid_density * particle->volume() * immersed;

	particle->acceleration() += force / particle->mass();
}

void ThrustForceGenerator::applyForceInWorld(physx::PxRigidDynamic* body, double t)
{
	Vector3D g = Vector3D(0, _gravity, 0);

	float body_volume = (
		body->getWorldBounds().getExtents().x * 2*
		body->getWorldBounds().getExtents().y * 2*
		body->getWorldBounds().getExtents().z * 2
		);

	Vector3D force = -g * _fluid_density * body_volume;

	body->addForce(force.to_vec3());
}

void ThrustForceGenerator::applyForceInArea(physx::PxRigidDynamic* body, double t)
{
	float body_height = body->getWorldBounds().getExtents().y;

	float h = body->getGlobalPose().p.y;
	float h0 = _fluid_area_transform->p.y + _fluid_area->halfExtents.y;

	float immersed = 0;
	if (h - h0 > body_height * 0.5)
		immersed = 0;
	else if (h0 - h > body_height * 0.5)
		immersed = 1;
	else
		immersed = (h0 - h) / body_height + 0.5;

	Vector3D g = Vector3D(0, _gravity, 0);
	
	float body_volume = (
		body->getWorldBounds().getExtents().x * 2 *
		body->getWorldBounds().getExtents().y * 2 *
		body->getWorldBounds().getExtents().z * 2
		);
	
	Vector3D force = -g * _fluid_density * body_volume * immersed;

	body->addForce(force.to_vec3());
}
