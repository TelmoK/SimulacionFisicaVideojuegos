#pragma once
#include <PxPhysXConfig.h>

#include "../../RenderItems/DynamicEntity.h"

class EntitySystem;
class Particle;
class RenderBody;

class ForceGenerator
{
public:
	ForceGenerator(EntitySystem* particle_system) : _entity_system(particle_system) {}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	virtual void applyForce(Particle* particle, double t) = 0;
	virtual void applyForce(RenderBody* renderBody, double t) = 0;

	virtual void update(float t) {}

	void setActive(bool value) {
		_active = value;
	}

	bool active() {
		return _active;
	}

protected:
	EntitySystem* _entity_system = nullptr;

	bool _active = true;
};