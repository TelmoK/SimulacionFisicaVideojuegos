#include "RoundAreaRangeGenerator.h"

#include "../../RenderItems/Particle.h"
#include "../ParticleSystem.h"

RoundAreaRangeGenerator::RoundAreaRangeGenerator(ParticleSystem* particle_system, Particle* model_particle, float generation_period, int periodic_generated_particles, AreaType area_type, float radius)
	: ParticleGenerator(particle_system, model_particle, generation_period, periodic_generated_particles), 
	_radius(radius), _area_type(area_type)
{
	u_distribution = std::uniform_real_distribution<double>(0, _radius);
}

void RoundAreaRangeGenerator::generateParticles(int particle_num)
{
	for (int i = 0; i < particle_num; ++i)
	{
		Particle* spawned_particle = _model_particle->dynamic_copy();

		spawned_particle->transform().p.x += u_distribution(mt);

		if(_area_type == AreaType::SPHERE) 
			spawned_particle->transform().p.y += u_distribution(mt);

		spawned_particle->transform().p.z += u_distribution(mt);

		_particle_system->registerNewParticle(spawned_particle); // Se registra la partícula en el Sistema de Partículas
	}
}
