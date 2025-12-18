#include "EntitySystem.h"
#include "../RenderItems/Particle.h"

EntitySystem::~EntitySystem()
{
	for (auto p_register_it = _entity_registers.begin(); p_register_it != _entity_registers.end(); )
		p_register_it = deleteEntityGeneration((*p_register_it)->list_it);
}

void EntitySystem::registerNewEntity(DynamicEntity* entity, float life_time, bool inmortal)
{
	EntityGeneration* generation = new EntityGeneration();

	_entity_registers.push_back(generation);

	generation->entity = entity;
	generation->list_it = std::prev(_entity_registers.end());
	generation->life_time = life_time;
	generation->inmortal = inmortal;
}

EntitySystem::EntityGeneration_It EntitySystem::deleteEntityGeneration(EntityGeneration_It entity_generation)
{
	EntityGeneration* generation = (*entity_generation);

	delete generation->entity;

	delete generation;

	auto next_gen_it = _entity_registers.erase(entity_generation);

	return next_gen_it;
}

void EntitySystem::cleanUpDeadEntities()
{
	for (auto p_register_it = _entity_registers.begin(); p_register_it != _entity_registers.end(); )
	{
		// Se gestiona la invalidación de iteradores setteando p_register_it
		// al elemento siguiente al borrado, si no se ha borrado simplemente se incrementa en uno
		if (!(*p_register_it)->inmortal && (*p_register_it)->life_time <= 0)
			p_register_it = deleteEntityGeneration((*p_register_it)->list_it);
		else
			++p_register_it;
	}
}

void EntitySystem::referenceParticleGenerator(std::shared_ptr<ParticleGenerator> particle_generator)
{
	_particle_generators.push_back(std::move(particle_generator));
}

void EntitySystem::referenceForceGenerator(std::shared_ptr<ForceGenerator> force_generator)
{
	_force_generators.push_back(std::move(force_generator));
}

void EntitySystem::referenceBodyGenerator(std::shared_ptr<BodyGenerator> body_generator)
{
	_body_generators.push_back(std::move(body_generator));
}

void EntitySystem::update(float t)
{
	cleanUpDeadEntities();

	// Gestión de los generadores de entidades
	for(std::shared_ptr<ParticleGenerator> particle_generator : _particle_generators)
		particle_generator->handleGenerationPeriod(t);

	for(std::shared_ptr<BodyGenerator> body_generator : _body_generators)
		body_generator->handleGenerationPeriod(t);

	// Gestión de los generadores de fuerza e integración de las entidades
	for (EntityGeneration* ent_generation : _entity_registers)
	{
		for (std::shared_ptr<ForceGenerator> force_generator : _force_generators)
			ent_generation->entity->receiveForceFrom(force_generator.get(), t);

		ent_generation->entity->update(t);
		
		if(!ent_generation->inmortal)
			ent_generation->life_time -= t;
	}
}
