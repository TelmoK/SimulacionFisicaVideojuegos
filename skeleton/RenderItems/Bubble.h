#pragma once
#include "Particle.h"

/*
* Partícula de aire que desaparece en la supercie del agua
*/
class Bubble : public Particle
{
public:
	Bubble(Vector3D position, float water_height, Vector3D velocity = Vector3D(),
		Vector3D acceleration = Vector3D(), float mass = 1, float volume = 1);
	~Bubble(){}

	Particle* dynamic_copy() override;

	void update(float t) override;

protected:

	float _water_height; // Guarda dónde está la superficie para desaparecer si llega a ella
};