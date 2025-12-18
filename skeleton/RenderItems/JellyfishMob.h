#pragma once
#include "RenderBody.h"

#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

#include <random>

class JellyfishMob : public RenderBody, public RenderItem
{
public:
	JellyfishMob(Vector3D position, physx::PxPhysics* gPhysics, physx::PxScene* gScene, const Vector4& _color);

	~JellyfishMob();

	void update(float t) override;

	void addFriction();

protected:
	physx::PxPhysics* _gPhysics;
	physx::PxScene* _gScene;

	Vector3D DEFAULT_SIZE = Vector3D(5,4,5);

	// Gestión del movimiento
	
	// Máxima velocidad angular que la medusa que la medusa intentará tener, si se pasa frenará la rotación
	float MAX_ANGULAR_SPEED = 2; 

	std::mt19937 mt; // Para mover en direcciones aleatorias y distintas intensidades
	std::uniform_real_distribution<double> u_distribution;

	float _impulse_count_down = 2; // Tiempo pendiente hasta el proximo impulso de la medusa
	float _impulse_period = 2; // Periodo de tiempo en el que la medusa se impulsa
	float _rotation_count_down = 2; // Tiempo que le queda a la medusa para dejar de rotar
	float _rotation_duration = 2; // Cuánto tiempo está rotando la medusa

	float _impulse_force = 1640000; // Magnitud de la fuerza que hace la medusa al impulsarse
	float _rotation_force = 5000; // Magnitu de la fuerza que hace rotar a la medusa

	Vector3D _last_applied_torque;

	bool is_rotating = false; // Para controlar que se aplica la fuerza de rotación una sola vez

	Vector3D _base_orientation = Vector3D(0, 1, 0); // Orientación en la que se impulsa la medusa sin rotaciones
};