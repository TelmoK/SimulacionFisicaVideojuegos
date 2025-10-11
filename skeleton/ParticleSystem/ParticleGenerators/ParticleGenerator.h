#pragma once

class ParticleSystem;
class Particle;

class ParticleGenerator
{
public:
	ParticleGenerator(Particle* model_particle) : _model_particle(model_particle){}

protected:

	/**
		Implements how particles are generated. This method *must use* registerNewParticle() function
	*/
	virtual void generateParticles(ParticleSystem* particle_system) = 0;

	Particle* _model_particle = nullptr;
};