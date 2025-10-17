#include "UniformParticleGenerator.h"

#include "../../RenderItems/Particle.h"
#include "../ParticleSystem.h"
#include <random>

UniformParticleGenerator::UniformParticleGenerator(ParticleSystem* particle_system, Particle* model_particle)
	: ParticleGenerator(particle_system, model_particle) 
{

}

void UniformParticleGenerator::generateParticles(unsigned int particle_num)
{
	std::mt19937 mt;
	std::uniform_real_distribution<double> u_distribution(0, 1);

	for (int i = 0; i < particle_num; ++i)
	{
		Particle* spawned_particle = _model_particle->dynamic_copy();

		spawned_particle->velocity().x += u_distribution(mt);
		spawned_particle->velocity().y += u_distribution(mt);
		spawned_particle->velocity().z += u_distribution(mt);

		spawned_particle->transform().p.x += u_distribution(mt);
		spawned_particle->transform().p.y += u_distribution(mt);

		_particle_system->registerNewParticle(spawned_particle); // Se registra la partícula en el Sistema de Partículas
	}
}
