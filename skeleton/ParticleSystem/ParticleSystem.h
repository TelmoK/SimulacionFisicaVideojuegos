#pragma once
#include <list>
#include <memory>

#include "../RenderItems/Particle.h"
#include "ForceGenerators/ForceGenerator.h"
#include "ParticleGenerators/ParticleGenerator.h"

class ParticleSystem
{
public:

	ParticleSystem() {}

	~ParticleSystem();

	/*
		Metodo usado por los ParticleGenerators para registrar en el sistema las partículas que generan
		y gestionar su actualización y destrucción desde él.
	*/
	void registerNewParticle(Particle* particle, float life_time = 10, bool inmortal = false);

	/*
		Mete en un vector un puntero a un generador de fuezas que usará en cada update() para aplicárselo
		a las partículas registradas.
	*/
	void referenceForceGenerator(std::shared_ptr<ForceGenerator> force_generator);

	/*
		Mete en un vector un puntero a un generador de partículas que usará en cada update() para que estos
	*/
	void referenceParticleGenerator(std::shared_ptr<ParticleGenerator> particle_generator);

	void update(float t);

private:

	/**
		Struct que monitoriza las partículas creadas y su iterador de la lista de generaciones. Es como
		una Variable de Sesión para cada partícula generada.
	*/
	struct ParticleGeneration
	{
		Particle* particle = nullptr;
		std::list<ParticleGeneration*>::iterator list_it;
		float life_time = 10;
		bool inmortal = false;
	};

	std::list<ParticleGeneration*> _particle_registers;

	using ParticleGeneration_It = std::list<ParticleGeneration*>::iterator;

	std::vector<std::shared_ptr<ForceGenerator>> _force_generators;

	std::vector<std::shared_ptr<ParticleGenerator>> _particle_generators;

	ParticleGeneration_It deleteParticleGeneration(ParticleGeneration_It particle_generation);

	void cleanUpDeadParticles();
};