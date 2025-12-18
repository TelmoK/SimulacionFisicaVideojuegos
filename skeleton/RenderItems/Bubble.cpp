#include "Bubble.h"

Bubble::Bubble(Vector3D position, float water_height, Vector3D velocity, Vector3D acceleration, float mass, float volume)
	: Particle(position, velocity, acceleration, mass, volume), _water_height(water_height)
{
	
}

Particle* Bubble::dynamic_copy()
{
	return new Bubble(Vector3D(_transform.p), _water_height, _velocity, _acceleration, _real_mass, _volume);
}

void Bubble::update(float t)
{
	Particle::update(t);

	// Lo único que diferencia a la burbuja de una partícula normal es que 
	// desaparece en la superficie
	if (_transform.p.y > _water_height)
		DeregisterRenderItem(this);
}
