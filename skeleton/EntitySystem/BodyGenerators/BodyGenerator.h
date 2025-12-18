#pragma once
#include <PxPhysics.h>

class EntitySystem;

class BodyGenerator
{
public:

	/*
		Si generation_period < 0 las partículas no se generan de forma periódica.
	*/
	BodyGenerator(EntitySystem* entity_system, physx::PxPhysics* gPhysics, physx::PxScene* gScene, float generation_period = 1, int periodic_generated_bodies = 1);

	~BodyGenerator();

	/**
		Implementa cómo se generan los cuerpos. Este método *debe usar* ParticleSystem::registerNewBody()
	*/
	virtual void generateBodies(int particle_num = 1) = 0;

	/*
		Gestiona la generación automática de partículas si generation_period > 0
	*/
	void handleGenerationPeriod(float t);

	void setGenerationPeriod(float value) {
		_generation_period = value;
	}

	void setGenerationCooldown(float value) {
		_generation_cooldown = value;
	}

	void setPeriodicGeneratedBodies(int value) {
		_periodic_generated_particles = value;
	}

protected:

	EntitySystem* _entity_system = nullptr;
	physx::PxPhysics* _gPhysics; 
	physx::PxScene* _gScene;

	float _generation_period;

	float _generation_cooldown;

	int _periodic_generated_particles;
};