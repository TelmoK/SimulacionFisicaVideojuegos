#include "UniformParticleGenerator.h"

#include "../../RenderItems/Particle.h"
#include "../ParticleSystem.h"
#include <random>
#include <iostream>

UniformParticleGenerator::UniformParticleGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period, int periodic_generated_particles)
	: ParticleGenerator(particle_system, model_particle, generation_period, periodic_generated_particles)
{
	u_distribution = std::uniform_real_distribution<double>(0, 1);

	if (generation_period >= 0)
		generateParticles(_periodic_generated_particles);
}

void UniformParticleGenerator::generateParticles(int particle_num)
{
	for (int i = 0; i < particle_num; ++i)
	{
		Particle* spawned_particle = _model_particle->dynamic_copy();

		spawned_particle->velocity().x += u_distribution(mt);
		spawned_particle->velocity().y += u_distribution(mt);
		spawned_particle->velocity().z += u_distribution(mt);

		spawned_particle->transform().p.x += u_distribution(mt);
		spawned_particle->transform().p.y += u_distribution(mt);
		spawned_particle->transform().p.z += u_distribution(mt);

		_particle_system->registerNewParticle(spawned_particle); // Se registra la partícula en el Sistema de Partículas
	}
}
