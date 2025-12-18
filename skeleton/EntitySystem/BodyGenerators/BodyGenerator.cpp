#include "BodyGenerator.h"

BodyGenerator::BodyGenerator(
	EntitySystem* particle_system, physx::PxPhysics* gPhysics, physx::PxScene* gScene, 
	float generation_period, int periodic_generated_bodies
)
	: _entity_system(particle_system), _generation_period(generation_period), 
	_gPhysics(gPhysics), _gScene(gScene),
	_periodic_generated_particles(periodic_generated_bodies)
{
	if (generation_period > 0)
		_generation_cooldown = generation_period;
}

BodyGenerator::~BodyGenerator()
{
	
}

void BodyGenerator::handleGenerationPeriod(float t)
{
	if (_generation_period < 0) return;

	if (_generation_period != 0)
	{
		_generation_cooldown -= t;
		if (_generation_cooldown > 0) return;
	}

	generateBodies(_periodic_generated_particles);

	_generation_cooldown = _generation_period;
}