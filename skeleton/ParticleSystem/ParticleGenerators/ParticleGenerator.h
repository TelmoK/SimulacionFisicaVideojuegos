#pragma once

class ParticleSystem;
class Particle;

class ParticleGenerator
{
public:

	/*
		Si generation_period < 0 las part�culas no se generan de forma peri�dica.
	*/
	ParticleGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period = 1);

	~ParticleGenerator();

	/**
		Implementa c�mo se generan las part�cula. Este m�todo *debe usar* ParticleSystem::registerNewParticle() 
	*/
	virtual void generateParticles(unsigned int particle_num = 1) = 0;

	void handleGenerationPeriod(float t);

protected:

	ParticleSystem* _particle_system = nullptr;

	Particle* _model_particle = nullptr;

	float _generation_period;

	float _generation_cooldown;
};