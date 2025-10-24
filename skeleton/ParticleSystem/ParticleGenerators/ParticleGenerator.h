#pragma once

class ParticleSystem;
class Particle;

class ParticleGenerator
{
public:

	/*
		Si generation_period < 0 las partículas no se generan de forma periódica.
	*/
	ParticleGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period = 1, int periodic_generated_particles = 1);

	~ParticleGenerator();

	/**
		Implementa cómo se generan las partícula. Este método *debe usar* ParticleSystem::registerNewParticle() 
	*/
	virtual void generateParticles(int particle_num = 1) = 0;

	/*
		Gestiona la generación automática de partículas si generation_period > 0
	*/
	void handleGenerationPeriod(float t);

protected:

	ParticleSystem* _particle_system = nullptr;

	Particle* _model_particle = nullptr;

	float _generation_period;

	float _generation_cooldown;

	int _periodic_generated_particles;
};