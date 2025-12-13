#include "Submarine.h"

#include "../Particle.h"
#include "../Projectile.h"
#include "../../ParticleSystem/ParticleSystem.h"
#include "../../ParticleSystem/ParticleGenerators/UniformParticleGenerator.h"
#include "../../ParticleSystem/ForceGenerators/GravityForceGenerator.h"
#include "../../RenderUtils.hpp"

Submarine::Submarine(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3D position, ParticleSystem* world_particle_sys)
	: _gPhysics(gPhysics), _gScene(gScene), _world_particle_sys(world_particle_sys), 
	_motor_force(0), _camera_mode(CameraMode::SHELFIE)
{
	int width = 5, lenght = 8, height = 5;

	// Creación de la partícula que representa el centro de masas
	_center_mass = new Particle(position, Vector3D());

	_center_mass->mass() = 300000;//7880000;
	_center_mass->volume() = width * height * lenght;
	
	// Usamos la partícula como modelo, no se renderiza
	DeregisterRenderItem(_center_mass); 

	// Registramos la partícula del centro de masas para que sea afectada por las fuerzas
	// del sistema de partículas global
	_world_particle_sys->registerNewParticle(_center_mass, 0, true);

	// Renderización del cuerpo del submarino
	cabin = new RenderItem(CreateShape(physx::PxBoxGeometry(lenght, height, width)), &_center_mass->transform(), Vector4(1, 0, 1, 1));
	
	// Motor
	_motor_relative_pos = Vector3D(-lenght, 0, 0);

	_propellers = new SubmarinePropeller(8, _gPhysics);
	_propellers->core_piece()->setQuaternion(physx::PxQuat(physx::PxPi, Vector3(0, 1, 0))); // Giramos 180º
	_propellers->core_piece()->setPosition(position + _motor_relative_pos); // Las helices van en la cola del submarino

	// Posición de la cámara en primera persona
	_subarine_eye = Vector3D(lenght + 1, 0, 0);

	// Generadores de burbujas
	_motor_bubble_particle_model = new Particle(_motor_relative_pos, Vector3D(-2, 0, 0));

	_motor_particle_generator = std::make_shared<UniformParticleGenerator>(
		_world_particle_sys, _motor_bubble_particle_model, 0.15, 3
	);
	_world_particle_sys->referenceParticleGenerator(_motor_particle_generator);
}

Submarine::~Submarine()
{
	DeregisterRenderItem(cabin);
	delete cabin;
	//delete _center_mass;
}

void Submarine::update(float t)
{
	// Control de cámara
	//handleCameraFollow();

	// Fuerzas
	applyMotorForce(t);

	// Fuerzas
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
				_gPhysics, _gScene, GetCamera()->getEye(),
				GetCamera()->getDir() * _projectileSpeed*3,
				0.5, 0.5
			));

		break;

	case 'T':
			_projectiles.push_back(new Projectile(
				_gPhysics, _gScene,
				_center_mass->position(),
				Vector3D(0, 1, 0) * _projectileSpeed * 2,
				1, 0.7
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

void Submarine::applyMotorForce(float t)
{
	// Vector3D force = _center_mass->transform().q.rotate((-_motor_relative_pos.normalized()).to_vec3() * _motor_force);
	Vector3D motor_torque = (_center_mass->transform().q * _propellers->core_piece()->transform().q).rotate(_propellers->base_orientation.to_vec3()) * 10000;//Vector3D(-10000, 0, 0);

	// Aplicación de las fuerzas y obtención de reacciones
	auto reaction_forces = _propellers->propagateForces({ Vector3D(), motor_torque, Vector3D(), _propellers->core_piece()->transform().p });

	// Cálculo de las fuerzas totales
	Vector3D total_torque = (motor_torque + reaction_forces.torque);
	Vector3D total_linaer_force = reaction_forces.force;

	// Cálulo de las aceleraciones

	// Las rotaciones se aplica en los ejes principales de las hélices, por lo que nos basta con la diagonal
	// del tensor de inercia inverso
	Vector3D propellers_inv_inertia_diagonal = _propellers->getInvInerceTensorDiagonal();

	// Aceleración angular
	Vector3D angular_acceleration = Vector3D(
		propellers_inv_inertia_diagonal.x * total_torque.x,
		propellers_inv_inertia_diagonal.y * total_torque.y,
		propellers_inv_inertia_diagonal.z * total_torque.z
	);

	// Aplicando aceleración
	Vector3D new_angular_velocity = _propellers->core_piece()->angular_velocity() + angular_acceleration;
	_center_mass->acceleration() += total_linaer_force / (_center_mass->mass()*0 + _propellers->mass());

	std::cout << "Rot " << new_angular_velocity.to_str() << "\n";
	std::cout << "Vel " << (total_linaer_force ).to_str() << "\n\n";

	// Moviendo la pieza
	_propellers->core_piece()->propagateMotionEffect({
		_propellers->core_piece()->transform().p, // Centro del movimiento
		(_motor_relative_pos.to_vec3() + _center_mass->position()) - _propellers->core_piece()->transform().p, // Desplazamiento lineal
		new_angular_velocity * t // Rotación
		});

	if (_motor_force < 100)
		_motor_particle_generator->setGenerationPeriod(-1);
	else
		_motor_particle_generator->setGenerationPeriod(0.15);
}
