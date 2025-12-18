#pragma once
#include "WindForceGenerator.h"
#include "../../RenderItems/Particle.h"

class TornadoForceGenerator : public ForceGenerator
{
public:
	TornadoForceGenerator(EntitySystem* particle_system, Vector3D tornado_eye, Vector3D wind_velocity, float k1, float k2, float tornadoForce)
		: ForceGenerator(particle_system), tornado_eye(tornado_eye), wind_velocity(wind_velocity), 
		k1(k1), k2(k2), tornadoForce(tornadoForce) 
	{
	}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(Particle* particle, double t) override
	{
		if (!_active) return;

		wind_velocity = Vector3D(
			particle->transform().p.x - tornado_eye.x,
			50 - (particle->transform().p.y - tornado_eye.y),
			-(particle->transform().p.z - tornado_eye.z)
		) * tornadoForce;

		Vector3D velocity_diff = (wind_velocity - particle->velocity());

		Vector3D force = velocity_diff * k1 + velocity_diff * velocity_diff.magnitude() * k2;

		particle->acceleration() += force / particle->mass();
	}

	/*
		Aplica la fuerza en un cuerpo que le pase el sistema de entidades.
		t es el delta time.
	*/
	void applyForce(RenderBody* renderBody, double t) override
	{
		if (!_active) return;
		
		physx::PxRigidDynamic* body = renderBody->body();

		wind_velocity = Vector3D(
			body->getGlobalPose().p.x - tornado_eye.x,
			50 - (body->getGlobalPose().p.y - tornado_eye.y),
			-(body->getGlobalPose().p.z - tornado_eye.z)
		) * tornadoForce;

		Vector3D velocity_diff = (wind_velocity - body->getLinearVelocity());

		Vector3D force = velocity_diff * k1 + velocity_diff * velocity_diff.magnitude() * k2;

		body->addForce(force.to_vec3());
	}

protected:

	Vector3D tornado_eye;
	Vector3D wind_velocity;
	float k1;
	float k2;
	float tornadoForce;
};