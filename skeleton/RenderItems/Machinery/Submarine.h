#pragma once
#include "../../RenderUtils.hpp"
#include "../../Utils/Vector3D.h"
#include <list>
#include <memory>

#include "SubmarinePropeller.h"
#include "IndustrialPieces/PropellerBladePiece.h"

class Particle;
class Projectile;
class ParticleSystem;
class UniformParticleGenerator;

class Submarine
{
public:
	enum CameraMode { THIRD_PERSON, SHELFIE, FIRST_PERSON, DETACHED };

	Submarine(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3D position, ParticleSystem* world_particle_sys);

	~Submarine();

	void applyMotorForce(float t);

	void handleCameraFollow();

	void update(float t);

	void keyPress(unsigned char key);

private:

	// Comportamiento con la física de la escena
	physx::PxPhysics* _gPhysics;
	physx::PxScene* _gScene;
	ParticleSystem* _world_particle_sys;

	// Gestión de  fuerzas que recibe el submarino
	Particle* _center_mass; // El centro de masas está representado por una partícula (no visible)

	// Partes del submarino
	RenderItem* cabin;
	SubmarinePropeller* _propellers; // Hélices
	PropellerBladePiece* _rudder; // Timón (aletas traseras que rotan el submarino)

	Vector3D _motor_relative_pos; // Posición de las hélices respecto al centro de masas
	Vector3D _rudder_relative_pos; // Posición del timón respecto al centro de masas

	/*temp*/ float _motor_force;

	// Control de la cámara
	CameraMode _camera_mode;
	Vector3D _subarine_eye;

	// Proyectiles 
	std::list<Projectile*> _projectiles;
	float _projectileSpeed = 30;

	// Efectos visuales
	Particle* _motor_bubble_particle_model; // La partícula modelo donde se van a generar las bubujas por el motor
	Particle* _tank_bubble_particle_model; // La partícula modelo donde de la que se generan burbujas al hundirse
	std::shared_ptr<UniformParticleGenerator> _motor_particle_generator;
};