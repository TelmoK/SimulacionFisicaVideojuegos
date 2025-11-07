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
	float surface_y_pos = _fluid_area_transform->p.y + _fluid_area->halfExtents.y;

	if (surface_y_pos < particle->position().y) { 
		return; 
	}

	applyForceInWorld(particle, t);
}
