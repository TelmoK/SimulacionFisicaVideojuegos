#pragma once
#include <list>
#include <memory>

#include "../RenderItems/Particle.h"
#include "ForceGenerators/ForceGenerator.h"
#include "ParticleGenerators/ParticleGenerator.h"
#include "BodyGenerators/BodyGenerator.h"

class EntitySystem
{
public:

	EntitySystem() {}

	~EntitySystem();

	/*
		Metodo usado por los ParticleGenerators para registrar en el sistema las partículas que generan
		y gestionar su actualización y destrucción desde él.
	*/
	void registerNewParticle(Particle* particle, float life_time = 2, bool inmortal = false);

	/*
		Metodo usado por los BodyGenerators para registrar en el sistema los cuerpos dinámicos que generan 
		y gestionar su actualización y destrucción desde él.
	*/
	void registerNewBody(
		physx::PxRigidDynamic* dynamicBody, RenderItem* bodyRenderItem, 
		float life_time = 20, bool inmortal = true);

	/*
		Mete en un vector un puntero a un generador de fuezas que usará en cada update() para aplicárselo
		a las partículas registradas.
	*/
	void referenceForceGenerator(std::shared_ptr<ForceGenerator> force_generator);

	/*
		Mete en un vector un puntero a un generador de partículas que usará en cada update() para que estos
		creen las partículas.
	*/
	void referenceParticleGenerator(std::shared_ptr<ParticleGenerator> particle_generator);

	/*
		Mete en un vector un puntero a un generador de cuerpos que usará en cada update() para que estos
		creen los cuerpos.
	*/
	void referenceBodyGenerator(std::shared_ptr<BodyGenerator> body_generator);

	void update(float t);

private:

	/**
		Struct que monitoriza las partículas y los sólidos rígidos creados y su iterador 
		de la lista de generaciones. Es como una Variable de Sesión para cada objeto físico
		generado.
	*/
	struct EntityGeneration
	{
		Particle* particle = nullptr;
		// o
		physx::PxRigidDynamic* dynamicBody = nullptr;
		RenderItem* bodyRenderItem = nullptr; // El RendeItem que renderiza el cuerpo

		std::list<EntityGeneration*>::iterator list_it;
		float life_time = 5;
		bool inmortal = false;
	};

	std::list<EntityGeneration*> _entity_registers;

	using EntityGeneration_It = std::list<EntityGeneration*>::iterator;

	std::vector<std::shared_ptr<ForceGenerator>> _force_generators;

	std::vector<std::shared_ptr<ParticleGenerator>> _particle_generators;

	std::vector<std::shared_ptr<BodyGenerator>> _body_generators;

	EntityGeneration_It deleteEntityGeneration(EntityGeneration_It entity_generation);

	void cleanUpDeadEntities();
};