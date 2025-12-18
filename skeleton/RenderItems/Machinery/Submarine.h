#pragma once
#include "../../RenderUtils.hpp"
#include "../../Utils/Vector3D.h"
#include <list>
#include <memory>

#include "SubmarinePropeller.h"
#include "IndustrialPieces/PropellerBladePiece.h"

class Particle;
class Bubble;
class Projectile;
class EntitySystem;
class UniformParticleGenerator;

class Submarine
{
public:
	enum CameraMode { THIRD_PERSON, SHELFIE, FIRST_PERSON, DETACHED };

	Submarine(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3D position, EntitySystem* world_particle_sys);

	~Submarine();

	void applyMotorForce(float t);

	void handleCameraFollow();

	void update(float t);

	void keyPress(unsigned char key);

	void fillAirTank(float quantity);

	void switchRudder();

	void setMassSpaceInvInertiaTensor();

	float getDensity();

	Particle* center_mass() {
		return _center_mass;
	}

private:

	// Comportamiento con la física de la escena
	physx::PxPhysics* _gPhysics;
	physx::PxScene* _gScene;
	EntitySystem* _world_particle_sys;

	// Gestión de  fuerzas que recibe el submarino
	Particle* _center_mass; // El centro de masas está representado por una partícula (no visible)

	// Partes del submarino
	RenderItem* cabin;
	SubmarinePropeller* _propellers; // Hélices
	PropellerBladePiece* _rudder; // Timón (aletas traseras que rotan el submarino)

	Vector3D BODY_SIZE = Vector3D(8, 5, 5);

	Vector3D _submarine_up = Vector3D(0, 1, 0);
	Vector3D _motor_relative_pos; // Posición de las hélices respecto al centro de masas
	Vector3D _rudder_relative_pos; // Posición del timón respecto al centro de masas

	physx::PxQuat _motor_initial_quaternion;
	physx::PxQuat _motor_spin_phase_quaternion; // Para guardar la rotación de las hélices en su propio eje
	physx::PxQuat _rudder_initial_quaternion;

	// Las rotaciones se aplica en los ejes principales de las hélices, por lo que nos basta con la diagonal
	// del tensor de inercia inverso
	Vector3D _propellers_inv_inertia_diagonal;
	Vector3D _submarine_inv_inertia_diagonal;

	// Fuerzas y valores para el movimiento
	float _motor_force;
	float _base_submarine_mass; // Masa del submarino con las cámarase de lastre vacías
	float _air_tank_capacity;   // Capacidad en volumen de las camaras de lastre
	float _air_tank_volume = 0;
	int _rudder_dir = 1; // Orientación del timón (izquierda = 1, derecha = -1)

	// Control de la cámara
	CameraMode _camera_mode;
	Vector3D _subarine_eye;

	// Proyectiles 
	std::list<Projectile*> _projectiles;
	float _projectileSpeed = 90;

	// Efectos visuales
	Particle* _motor_bubble_particle_model; // La partícula modelo donde se van a generar las bubujas por el motor
	Particle* _tank_bubble_particle_model; // La partícula modelo donde de la que se generan burbujas al hundirse
	std::shared_ptr<UniformParticleGenerator> _motor_particle_generator;
};