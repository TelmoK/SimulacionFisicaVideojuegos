#include "ParticleSystem.h"

void ParticleSystem::registerNewParticle(Particle* particle)
{
	_particle_registers.push_back({ particle, _particle_registers.end() });
}

void ParticleSystem::deleteParticleGeneration(ParticleGeneration_It particle_generation)
{
	delete particle_generation->particle; // Delete the particle
	_particle_registers.erase(particle_generation);
}

void ParticleSystem::cleanUpDeadParticles()
{
	for (ParticleGeneration& p_generation : _particle_registers)
	{
		if (!p_generation.inmortal && p_generation.life_time <= 0)
			deleteParticleGeneration(p_generation.list_it);
	}
}

void ParticleSystem::update(float t)
{
	cleanUpDeadParticles();

	for (ParticleGeneration& p_generation : _particle_registers)
	{
		p_generation.particle->integrate(t);

		if(!p_generation.inmortal)
			p_generation.life_time -= t;
	}
}
