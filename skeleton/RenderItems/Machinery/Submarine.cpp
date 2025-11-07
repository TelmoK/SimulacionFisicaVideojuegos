#include "Submarine.h"

#include "../Particle.h"
#include "../Projectile.h"
#include "../../ParticleSystem/ParticleSystem.h"
#include "../../ParticleSystem/ParticleGenerators/UniformParticleGenerator.h"
#include "../../ParticleSystem/ForceGenerators/GravityForceGenerator.h"
#include "../../RenderUtils.hpp"

Submarine::Submarine(Vector3D position, ParticleSystem* world_particle_sys)
	: _world_particle_sys(world_particle_sys), _motor_force(0), _camera_mode(CameraMode::SHELFIE)
{
	int width = 5, lenght = 8, height = 5;

	_center_mass = new Particle(position, Vector3D());
	DeregisterRenderItem(_center_mass); // Usamos la partícula como modelo, no se renderiza
	_world_particle_sys->registerNewParticle(_center_mass, 0, true);

	cabin = new RenderItem(CreateShape(physx::PxBoxGeometry(lenght, height, width)), &_center_mass->transform(), Vector4(1, 0, 1, 1));
	
	_motor_relative_pos = Vector3D(-lenght, 0, 0);
	_subarine_eye = Vector3D(lenght + 1, 0, 0);

	_motor_bubble_particle_model = new Particle(_motor_relative_pos, Vector3D(-2, 0, 0));
	
	_center_mass->mass() = 190000;//7880000;
	_center_mass->volume() = width * height * lenght;

	_motor_particle_generator = std::make_shared<UniformParticleGenerator>(
		_world_particle_sys, _motor_bubble_particle_model, 0.15, 3
	);
	_world_particle_sys->referenceParticleGenerator(_motor_particle_generator);

	// Las partícuias tienen su propio sistema de partículas para gestionarse
	_proyectile_particle_sys = std::make_unique<ParticleSystem>();
	_proyectile_particle_sys->referenceForceGenerator(
		std::make_shared<GravityForceGenerator>(_proyectile_particle_sys.get(), -9.8)
	);
}

Submarine::~Submarine()
{
	DeregisterRenderItem(cabin);
	delete cabin;
	//delete _center_mass;
}

void Submarine::update(float t)
{	
	_proyectile_particle_sys->update(t);

	// Control de cámara
	handleCameraFollow();

	// Fuerzas
	applyMotorForce();

	// Movimiento
	// Se gestiona desde la partícula de centro de masas

	_motor_bubble_particle_model->transform().p = _center_mass->transform().p + _motor_relative_pos.to_vec3();
}

void Submarine::keyPress(unsigned char key)
{
	switch (toupper(key))
	{
	case 'P':
		if(_camera_mode == CameraMode::FIRST_PERSON)
			_projectiles.push_back(new Projectile(
				_proyectile_particle_sys.get(), GetCamera()->getEye(),
				GetCamera()->getDir() * _projectileSpeed, Vector3D(), 2
			));
		break;

	case 'T':
			_projectiles.push_back(new Projectile(
				_proyectile_particle_sys.get(), _center_mass->position(),
				Vector3D(0, 1, 0) * _projectileSpeed * 2, Vector3D(), 10
			));
		break;

	case 'X':
		_motor_force += 800;
		break;

	case 'Z':
		_motor_force -= 1200;
		if (_motor_force < 0) _motor_force = 0;
		break;

	case 'L':
		_center_mass->volume() += 50;
		if (_center_mass->volume() > 450) _center_mass->volume() = 450;
		break;

	case 'K':
		_center_mass->volume() -= 50;
		if (_center_mass->volume() < 50) _center_mass->volume() = 50;
		break;

	case '1':
		_camera_mode = CameraMode::FIRST_PERSON;
		GetCamera()->setDir(Vector3D(1, 0, 0).to_vec3());
		break;

	case '2':
		_camera_mode = CameraMode::SHELFIE;
		GetCamera()->setDir(-Vector3D(1, 1, 1).normalized().to_vec3());
		break;

	case '3':
		_camera_mode = CameraMode::THIRD_PERSON;
		GetCamera()->setDir(-Vector3D(-15, 20, 0).normalized().to_vec3());
		break;

	case '4':
		_camera_mode = CameraMode::DETACHED;

		break;

	default:
		break;
	}
}

void Submarine::handleCameraFollow()
{

	if (_camera_mode == FIRST_PERSON)
	{
		GetCamera()->setEye(_center_mass->transform().p + _subarine_eye.to_vec3());
		return;
	}

	if (_camera_mode == SHELFIE)
	{
		Vector3D camPos = _center_mass->transform().p + Vector3D(15, 15, 15).to_vec3();
		GetCamera()->setEye(camPos.to_vec3());
		return;
	}

	if (_camera_mode == THIRD_PERSON)
	{
		Vector3D camPos = _center_mass->transform().p + Vector3D(-15, 25, 0).to_vec3();
		GetCamera()->setEye(camPos.to_vec3());
		return;
	}
}

void Submarine::applyMotorForce()
{
	Vector3D force = _center_mass->transform().q.rotate((-_motor_relative_pos.normalized()).to_vec3() * _motor_force);

	_center_mass->acceleration() += force / _center_mass->mass();

	if (_motor_force < 100)
		_motor_particle_generator->setGenerationPeriod(-1);
	else
		_motor_particle_generator->setGenerationPeriod(0.15);
}
