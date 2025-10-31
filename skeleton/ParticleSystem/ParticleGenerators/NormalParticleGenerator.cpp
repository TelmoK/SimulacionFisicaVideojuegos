#include "NormalParticleGenerator.h"

#include "../../RenderItems/Particle.h"
#include "../ParticleSystem.h"
#include <random>
#include <iostream>

NormalParticleGenerator::NormalParticleGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period, int periodic_generated_particles)
	: ParticleGenerator(particle_system, model_particle, generation_period, periodic_generated_particles)
{
	n_distribution = std::normal_distribution<double>(0, 1);

	if (generation_period >= 0)
		generateParticles(_periodic_generated_particles);
}

void NormalParticleGenerator::generateParticles(int particle_num)
{
	for (int i = 0; i < particle_num; ++i)
	{
		Particle* spawned_particle = _model_particle->dynamic_copy();

		spawned_particle->velocity().x += n_distribution(mt);
		spawned_particle->velocity().y += n_distribution(mt);
		spawned_particle->velocity().z += n_distribution(mt);

		spawned_particle->transform().p.x += n_distribution(mt);
		spawned_particle->transform().p.y += n_distribution(mt);

		_particle_system->registerNewParticle(spawned_particle); // Se registra la partícula en el Sistema de Partículas
	}
}
