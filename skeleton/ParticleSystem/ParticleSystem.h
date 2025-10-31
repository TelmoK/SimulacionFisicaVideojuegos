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

	/*
		Metodo usado por los ParticleGenerators para registrar en el sistema las part�culas que generan
		y gestionar su actualizaci�n y destrucci�n desde �l.
	*/
	void registerNewParticle(Particle* particle);

	/*
		Mete en un vector un puntero a un generador de fuezas que usar� en cada update() para aplic�rselo
		a las part�culas registradas.
	*/
	void referenceForceGenerator(std::shared_ptr<ForceGenerator> force_generator);

	/*
		Mete en un vector un puntero a un generador de part�culas que usar� en cada update() para que estos
	*/
	void referenceParticleGenerator(std::shared_ptr<ParticleGenerator> particle_generator);

	void update(float t);

private:

	/**
		Struct que monitoriza las part�culas creadas y su iterador de la lista de generaciones. Es como
		una Variable de Sesi�n para cada part�cula generada.
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