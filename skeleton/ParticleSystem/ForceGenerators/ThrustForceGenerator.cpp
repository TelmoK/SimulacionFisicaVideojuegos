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

	/*if (surface_y_pos < particle->position().y) {
		return;
	}*/

	Vector3D g = Vector3D(0, _gravity, 0);

	Vector3D force = -g * _fluid_density * particle->volume() * immersed;

	particle->acceleration() += force / particle->mass();

	applyForceInWorld(particle, t);
}
