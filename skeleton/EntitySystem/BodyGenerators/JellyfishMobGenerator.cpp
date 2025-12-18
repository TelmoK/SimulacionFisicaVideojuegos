#include "JellyfishMobGenerator.h"

#include "../../RenderItems/JellyfishMob.h"
#include "../../RenderItems/Particle.h"
#include "../EntitySystem.h"

JellyfishMobGenerator::JellyfishMobGenerator(
	EntitySystem* entity_system, physx::PxPhysics* gPhysics, physx::PxScene* gScene, 
	Particle* model_particle, float generation_radius, float water_height,
	float generation_period, int periodic_generated_bodies
)
	: BodyGenerator(entity_system, gPhysics, gScene, generation_period, periodic_generated_bodies),
	_model_particle(model_particle), _water_height(water_height), _generation_radius(generation_radius)
{
	n_distribution = std::normal_distribution<double>(0, 1);

	if (generation_period >= 0)
		generateBodies(_periodic_generated_particles);
}

void JellyfishMobGenerator::generateBodies(int body_num)
{
	for (int i = 0; i < body_num; ++i)
	{
		Vector3D position = _model_particle->position();

		position.x += n_distribution(mt) * _generation_radius;
		position.y += n_distribution(mt) * _generation_radius;
		position.z += n_distribution(mt) * _generation_radius;

		if (position.y > _water_height) // No generamos medusas fuera del agua
			position.y = _water_height * 0.5 - 5;

		float red_color = n_distribution(mt);
		float green_color = n_distribution(mt);
		float blue_color = n_distribution(mt);

		if (red_color < 0.5) blue_color = 0.5;
		if (green_color > 0.35) blue_color = 0.35;
		if (blue_color < 0.5) blue_color = 0.5;

		JellyfishMob* jellyfish = new JellyfishMob(position, _gPhysics, _gScene, { 0.917, 0.237, blue_color, 1 });
		
		// Se registra el cuerpo en el Sistema de Entidades
		_entity_system->registerNewEntity(jellyfish, 10, false); 
	}
}