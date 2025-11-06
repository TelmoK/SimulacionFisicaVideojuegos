#pragma once
#include "../../RenderUtils.hpp"
#include "../../Utils/Vector3D.h"
#include <list>

class Particle;

class Submarine
{
public:

	Submarine();

	~Submarine();

	void applyMotorForce();
	void applyThrustForce();
	void applyGravity();

	void handleCameraFollow();

	void handleProyectilesLife(float t);

	void update(float t);

	void keyPress(unsigned char key);

	Particle* motor_bubble() {
		return _motor_bubble_particle_model;
	}

private:

	/*temp*/ float _motor_force;

	Particle* _center_mass; // El centro de masas está representado por una partícula (no visible)

	// Objeto renderizado
	RenderItem* cabin;
	
	// Atributos que referencian a la partícula de centro de masas
	/*physx::PxTransform* _transform;
	Vector3D _acceleration;
	Vector3D _velocity;
	float _mass;
	float _volume;*/

	// Componentes de input motor
	Vector3D _motor_torque;
	Vector3D _motor_relative_pos;

	Particle* _motor_bubble_particle_model; // La partícula modelo donde se van a generar las bubujas por el motor
	Particle* _tank_bubble_particle_model; // La partícula modelo donde de la que se generan burbujas al hundirse

	bool _camera_follow = false;
	bool _first_person_mode = false;
	Vector3D _subarine_eye;

	std::list<Particle*> _projectiles;
	float _projectileSpeed = 30;
};