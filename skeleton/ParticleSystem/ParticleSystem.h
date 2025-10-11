#pragma once
#include <list>
#include "../RenderItems/Particle.h"

class ParticleSystem
{
public:

	ParticleSystem() {}

	void registerNewParticle(Particle* particle);

private:

	/**
		Struct that keeps track of the generated particles and their iterator to the generator's list
	*/
	struct ParticleGeneration
	{
		Particle* particle;
		std::list<ParticleGeneration>::iterator list_it;
	};

	std::list<ParticleGeneration> particle_registers;

	using ParticleGeneration_It = std::list<ParticleGeneration>::iterator;

	void deleteParticleGeneration(ParticleGeneration_It particle_generation);
};