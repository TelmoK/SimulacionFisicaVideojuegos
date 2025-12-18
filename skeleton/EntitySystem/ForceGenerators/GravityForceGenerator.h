#pragma once
#include "ForceGenerator.h"
#include "../../RenderItems/Particle.h"

class GravityForceGenerator : public ForceGenerator
{
public:
	GravityForceGenerator(EntitySystem* particle_system, float gravityForce) 
		: ForceGenerator(particle_system), gravityForce(gravityForce){}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		particle->acceleration().y += gravityForce;
	}

	/*
		Aplica la fuerza en un cuerpo que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(RenderBody* renderBody, double t) override
	{
		if (!_active) return;
		
		physx::PxRigidDynamic* body = renderBody->body();

		body->addForce(Vector3(0, 1, 0) * gravityForce * body->getMass()); // F = m * a
	}

protected:

	float gravityForce;
};