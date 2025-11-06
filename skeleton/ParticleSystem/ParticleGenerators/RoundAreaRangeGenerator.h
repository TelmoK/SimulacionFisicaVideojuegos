#include "ParticleGenerator.h"
#include <random>

class RoundAreaRangeGenerator : public ParticleGenerator
{
public:
	enum AreaType { CIRCLE, SPHERE };

	RoundAreaRangeGenerator(
		ParticleSystem* particle_system, Particle* model_particle, float generation_period = 1,
		int periodic_generated_particles = 1, AreaType _area_type = CIRCLE, float radius = 10
	);

	void generateParticles(int particle_num) override;

private:
	std::mt19937 mt;
	std::uniform_real_distribution<double> u_distribution;

	AreaType _area_type;
	float _radius;
};