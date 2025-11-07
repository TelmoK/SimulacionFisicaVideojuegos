#include "Projectile.h"

Projectile::Projectile(ParticleSystem* p_sys, Vector3D position, Vector3D velocity, Vector3D acceleration, float mass, float volume)
	: Particle(p_sys, position, velocity, acceleration, mass)
{
}

Projectile::Projectile(Vector3D position, Vector3D velocity, Vector3D acceleration, float mass, float volume)
	: Particle(position, velocity, acceleration, mass)
{
}
