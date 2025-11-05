#include "ParticleSystem.h"
#include "../RenderItems/Particle.h"

ParticleSystem::~ParticleSystem()
{
	for (auto p_register_it = _particle_registers.begin(); p_register_it != _particle_registers.end(); )
		p_register_it = deleteParticleGeneration((*p_register_it)->list_it);
}

void ParticleSystem::registerNewParticle(Particle* particle)
{
	ParticleGeneration* generation = new ParticleGeneration();
	
	_particle_registers.push_back(generation);
	
	generation->particle = particle;
	generation->list_it = std::prev(_particle_registers.end());
}

ParticleSystem::ParticleGeneration_It ParticleSystem::deleteParticleGeneration(ParticleGeneration_It particle_generation)
{
	ParticleGeneration* generation = (*particle_generation);

	auto next_gen_it = _particle_registers.erase(particle_generation);

	delete generation->particle;
	delete generation;

	return next_gen_it;
}

void ParticleSystem::cleanUpDeadParticles()
{
	for (auto p_register_it = _particle_registers.begin(); p_register_it != _particle_registers.end(); )
	{
		// Se gestiona la invalidación de iteradores setteando p_register_it
		// al elemento siguiente al borrado, si no se ha borrado simplemente se incrementa en uno
		if (!(*p_register_it)->inmortal && (*p_register_it)->life_time <= 0)
			p_register_it = deleteParticleGeneration((*p_register_it)->list_it);
		else
			++p_register_it;
	}
}

void ParticleSystem::referenceParticleGenerator(std::shared_ptr<ParticleGenerator> particle_generator)
{
	_particle_generators.push_back(std::move(particle_generator));
}

void ParticleSystem::referenceForceGenerator(std::shared_ptr<ForceGenerator> force_generator)
{
	_force_generators.push_back(std::move(force_generator));
}

void ParticleSystem::update(float t)
{
	cleanUpDeadParticles();

	for(std::shared_ptr<ParticleGenerator> particle_generator : _particle_generators)
		particle_generator->handleGenerationPeriod(t);

	for (ParticleGeneration* p_generation : _particle_registers)
	{
		for(std::shared_ptr<ForceGenerator> force_generator : _force_generators)
			force_generator->applyForce(p_generation->particle, t);

		p_generation->particle->integrate(t);
		
		if(!p_generation->inmortal)
			p_generation->life_time -= t;
	}
}
