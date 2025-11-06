#include "Submarine.h"

#include "../Particle.h"
#include "../../ParticleSystem/ParticleSystem.h"
#include "../../ParticleSystem/ParticleGenerators/UniformParticleGenerator.h"
#include "../../RenderUtils.hpp"

Submarine::Submarine(ParticleSystem* world_particle_sys)
	: _world_particle_sys(world_particle_sys), _motor_force(0)
{
	Vector3D position = Vector3D();

	int width = 5, lenght = 8, height = 5;

	_center_mass = new Particle(position, Vector3D());
	DeregisterRenderItem(_center_mass); // Usamos la partícula como modelo, no se renderiza
	_world_particle_sys->registerNewParticle(_center_mass, 0, true);

	cabin = new RenderItem(CreateShape(physx::PxBoxGeometry(lenght, height, width)), &_center_mass->transform(), Vector4(1, 0, 1, 1));
	
	_motor_relative_pos = Vector3D(-lenght, 0, 0);
	_subarine_eye = Vector3D(lenght + 1, 0, 0);

	_motor_bubble_particle_model = new Particle(_motor_relative_pos, Vector3D(0, 0, 0));
	
	_center_mass->mass() = 100000;
	_center_mass->volume() = 100;

	_world_particle_sys->referenceParticleGenerator(std::make_shared<UniformParticleGenerator>(_world_particle_sys, _motor_bubble_particle_model, 0.2, 3));
}

Submarine::~Submarine()
{
	DeregisterRenderItem(cabin);
	delete cabin;
	//delete _center_mass;
}

void Submarine::update(float t)
{
	handleProyectilesLife(t);
	
	// Control de cámara
	handleCameraFollow();

	// Fuerzas
	//applyMotorForce();

	//applyThrustForce();

	//applyGravity();

	// Movimiento
	/*Vector3D new_position = Vector3D(_center_mass->transform().p) + _center_mass->velocity() * t;
	_center_mass->transform().p = new_position.to_vec3();

	_center_mass->velocity() = _center_mass->velocity() + _center_mass->acceleration() * t;
	*/
	_motor_bubble_particle_model->transform().p = _center_mass->transform().p + _motor_relative_pos.to_vec3();

}

void Submarine::keyPress(unsigned char key)
{
	switch (toupper(key))
	{
	case 'P':
		if(_camera_follow && _first_person_mode)
			_projectiles.push_back(new Particle(GetCamera()->getEye(), GetCamera()->getDir() * _projectileSpeed, -9.8));
		break;
	case 'X':
		_motor_force += 600;
		break;

	case 'Z':
		_motor_force -= 600;
		if (_motor_force < 0) _motor_force = 0;
		break;

	case 'L':
		_center_mass->volume() -= 50;
		if (_center_mass->volume() < 50) _center_mass->volume() = 50;
		break;

	case 'K':
		_center_mass->volume() += 50;
		if (_center_mass->volume() > 200) _center_mass->volume() = 200;
		break;

	case 'C':
		_camera_follow = !_camera_follow;
		if (_camera_follow) {
			_first_person_mode = false;
			GetCamera()->setDir((_center_mass->transform().p - GetCamera()->getEye()).getNormalized());
		}
		break;

	case 'O':
		_first_person_mode = !_first_person_mode;

		if(_first_person_mode)
			GetCamera()->setDir(_center_mass->transform().q.rotate(Vector3D(1, 0, 0).to_vec3()).getNormalized());
		break;

	default:
		break;
	}
}

void Submarine::handleCameraFollow()
{
	if (!_camera_follow) return;

	if (_first_person_mode)
	{
		GetCamera()->setEye(_center_mass->transform().p + _subarine_eye.to_vec3());
	}
	else
	{
		Vector3D camPos = _center_mass->transform().p + Vector3D(15, 15, 15).to_vec3();
		GetCamera()->setEye(camPos.to_vec3());
	}
}

void Submarine::handleProyectilesLife(float t)
{
	for (Particle* projectile : _projectiles)
		projectile->integrate(t);

	for (auto proyectile_it = _projectiles.begin(); proyectile_it != _projectiles.end(); )
	{
		if (((*proyectile_it)->transform().p - GetCamera()->getEye()).magnitudeSquared() > 800) {
			delete* proyectile_it;
			proyectile_it = _projectiles.erase(proyectile_it);
		}
		else {
			++proyectile_it;
		}
	}
}

void Submarine::applyGravity()
{
	_center_mass->acceleration() += Vector3D(0, -9.8, 0);
}

void Submarine::applyMotorForce()
{
	Vector3D force = _center_mass->transform().q.rotate((-_motor_relative_pos.normalized()).to_vec3() * _motor_force);

	_center_mass->acceleration() += force / _center_mass->mass();
}

void Submarine::applyThrustForce()
{
	const float WATER_DENSITY = 997;
	Vector3D gravity = Vector3D(0, -9.8, 0);

	Vector3D force = -gravity * WATER_DENSITY * _center_mass->volume();

	_center_mass->acceleration() += force / _center_mass->mass();
}
