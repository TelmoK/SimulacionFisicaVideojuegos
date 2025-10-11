#include "UniformParticleGenerator.h"

#include "../../RenderItems/Particle.h"
#include <random>

void UniformParticleGenerator::generateParticles(ParticleSystem* particle_system)
{
	Particle* spawned_particle = _model_particle->dynamic_copy();

	std::mt19937 mt;
	std::uniform_real_distribution<double> u_distribution;
}
