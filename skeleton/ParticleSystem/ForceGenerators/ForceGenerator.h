#pragma once

class ParticleSystem;
class Particle;

class ForceGenerator
{
public:
	ForceGenerator(ParticleSystem* particle_system) : _particle_system(particle_system) {}

	/*
		Aplica la fuerza en una partícula que le pase el sistema de partículas.
		t es el delta time.
	*/
	virtual void applyForce(Particle* particle, double t) = 0;

	virtual void update(float t) {}

	void setActive(bool value) {
		_active = value;
	}

	bool active() {
		return _active;
	}

protected:
	ParticleSystem* _particle_system = nullptr;

	bool _active = true;
};