#pragma once
#include "Particle.h"

class Projectile : public Particle
{
public:
	Projectile(
		ParticleSystem* p_sys, Vector3D position, Vector3D velocity = Vector3D(), 
		Vector3D acceleration = Vector3D(), float mass = 1, float volume = 1);

	Projectile(Vector3D position, Vector3D velocity = Vector3D(), Vector3D acceleration = Vector3D(), 
		float mass = 1, float volume = 1);
};