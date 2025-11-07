#pragma once
#include "../../RenderUtils.hpp"
#include "../../Utils/Vector3D.h"
#include <list>
#include <memory>

class Particle;
class Projectile;
class ParticleSystem;
class UniformParticleGenerator;

class Submarine
{
public:
	enum CameraMode { THIRD_PERSON, SHELFIE, FIRST_PERSON, DETACHED };

	Submarine(Vector3D position, ParticleSystem* world_particle_sys);

	~Submarine();

	void applyMotorForce();

	void handleCameraFollow();

	void update(float t);

	void keyPress(unsigned char key);

private:

	ParticleSystem* _world_particle_sys;

	/*temp*/ float _motor_force;

	Particle* _center_mass; // El centro de masas está representado por una partícula (no visible)

	// Objeto renderizado
	RenderItem* cabin;

	// Componentes de input motor
	Vector3D _motor_torque;
	Vector3D _motor_relative_pos;

	Particle* _motor_bubble_particle_model; // La partícula modelo donde se van a generar las bubujas por el motor
	Particle* _tank_bubble_particle_model; // La partícula modelo donde de la que se generan burbujas al hundirse
	std::shared_ptr<UniformParticleGenerator> _motor_particle_generator;

	CameraMode _camera_mode;
	Vector3D _subarine_eye;

	std::list<Projectile*> _projectiles;
	float _projectileSpeed = 30;
	std::unique_ptr<ParticleSystem> _proyectile_particle_sys;
};