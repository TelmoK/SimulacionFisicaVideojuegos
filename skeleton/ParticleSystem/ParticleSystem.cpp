#include "ParticleSystem.h"

void ParticleSystem::deleteParticleGeneration(ParticleGeneration_It particle_generation)
{
	delete particle_generation->particle; // Delete the particle
	particle_registers.erase(particle_generation);
}

void ParticleSystem::registerNewParticle(Particle* particle)
{
	particle_registers.push_back({ particle, particle_registers.end() });
}