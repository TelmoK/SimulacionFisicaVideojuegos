#pragma once
#include "BodyGenerator.h"

#include <random>

class Particle;

class JellyfishMobGenerator : public BodyGenerator
{
public:
	JellyfishMobGenerator(EntitySystem* entity_system, physx::PxPhysics* gPhysics, physx::PxScene* gScene,
		Particle* model_particle, float generation_radius, float water_height, 
		float generation_period = 1, int periodic_generated_bodies = 1);

	void generateBodies(int body_num = 1) override;

protected:

	Particle* _model_particle = nullptr;
	float _generation_radius;
	float _water_height;

	std::mt19937 mt;
	std::normal_distribution<double> n_distribution;
};