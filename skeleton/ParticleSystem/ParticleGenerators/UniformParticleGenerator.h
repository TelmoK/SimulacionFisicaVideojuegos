#include "ParticleGenerator.h"

class UniformParticleGenerator : public ParticleGenerator
{
public:
	UniformParticleGenerator(Particle* model_particle) : ParticleGenerator(model_particle){}

	void generateParticles(ParticleSystem* particle_system) override;
};