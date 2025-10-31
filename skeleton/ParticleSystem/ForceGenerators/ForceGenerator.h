#pragma once

class ParticleSystem;
class Particle;

class ForceGenerator
{
public:
	ForceGenerator(ParticleSystem* particle_system) : _particle_system(particle_system) {}

	/*
		Aplica la fuerza en una part�cula que le pase el sistema de part�culas.
		t es el delta time.
	*/
	virtual void applyForce(Particle* particle, double t) = 0;

protected:
	ParticleSystem* _particle_system = nullptr;
};