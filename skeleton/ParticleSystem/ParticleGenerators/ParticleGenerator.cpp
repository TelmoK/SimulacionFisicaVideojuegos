#include "ParticleGenerator.h"

#include "../../RenderUtils.hpp"
#include "../../RenderItems/Particle.h"

ParticleGenerator::ParticleGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period, int periodic_generated_particles)
	: _particle_system(particle_system), _model_particle(model_particle),
	_generation_period(generation_period), _periodic_generated_particles(periodic_generated_particles)
{
	DeregisterRenderItem(model_particle);

	if (generation_period > 0)
		_generation_cooldown = generation_period;
}

ParticleGenerator::~ParticleGenerator()
{
	delete _model_particle;
}

void ParticleGenerator::handleGenerationPeriod(float t)
{
	if (_generation_period < 0) return;

	if (_generation_period != 0) 
	{
		_generation_cooldown -= t;
		if (_generation_cooldown > 0) return;
	}
		
	generateParticles(_periodic_generated_particles);

	_generation_cooldown = _generation_period;
}