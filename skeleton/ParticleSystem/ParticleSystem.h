#pragma once
#include <list>
#include "../RenderItems/Particle.h"

class ParticleSystem
{
public:

	ParticleSystem() {}

	void registerNewParticle(Particle* particle);

	void update(float t);

private:

	/**
		Struct que monitoriza las partículas creadas y su iterador de la lista de generaciones. Es como
		una Variable de Sesión para cada partícula generada.
	*/
	struct ParticleGeneration
	{
		Particle* particle;
		std::list<ParticleGeneration>::iterator list_it;
		float life_time;
		bool inmortal = false;
	};

	std::list<ParticleGeneration> _particle_registers;

	using ParticleGeneration_It = std::list<ParticleGeneration>::iterator;

	void deleteParticleGeneration(ParticleGeneration_It particle_generation);

	void cleanUpDeadParticles();
};