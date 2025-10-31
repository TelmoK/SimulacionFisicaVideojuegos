#pragma once

#include "ParticleGenerator.h"
#include <random>

class UniformParticleGenerator : public ParticleGenerator
{
public:
	UniformParticleGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period = 1, int periodic_generated_particles = 1);

	void generateParticles(int particle_num) override;

private:
	std::mt19937 mt;
	std::uniform_real_distribution<double> u_distribution;
};