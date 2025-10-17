#include "ParticleGenerator.h"

class UniformParticleGenerator : public ParticleGenerator
{
public:
	UniformParticleGenerator(ParticleSystem* particle_system, Particle* model_particle);

	void generateParticles(unsigned int particle_num) override;
};