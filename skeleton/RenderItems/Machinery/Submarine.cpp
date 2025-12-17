#include "Submarine.h"

#include "../Particle.h"
#include "../Projectile.h"
#include "../../ParticleSystem/EntitySystem.h"
#include "../../ParticleSystem/ParticleGenerators/UniformParticleGenerator.h"
#include "../../ParticleSystem/ForceGenerators/GravityForceGenerator.h"
#include "../../RenderUtils.hpp"

Submarine::Submarine(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3D position, EntitySystem* world_particle_sys)
	: _gPhysics(gPhysics), _gScene(gScene), _world_particle_sys(world_particle_sys), 
	_motor_force(0), _camera_mode(CameraMode::SHELFIE)
{
	// Creación de la partícula que representa el centro de masas
	_center_mass = new Particle(position, Vector3D());

	_center_mass->mass() = 30000;//7880000;
	_center_mass->volume() = BODY_SIZE.x * BODY_SIZE.y * BODY_SIZE.z * 0.5;
	
	// Usamos la partícula como modelo, no se renderiza
	DeregisterRenderItem(_center_mass); 

	// Registramos la partícula del centro de masas para que sea afectada por las fuerzas
	// del sistema de partículas global
	_world_particle_sys->registerNewParticle(_center_mass, 0, true);

	// Renderización del cuerpo del submarino
	cabin = new RenderItem(CreateShape(physx::PxBoxGeometry(BODY_SIZE.x, BODY_SIZE.y, BODY_SIZE.z)), &_center_mass->transform(), Vector4(1, 0, 1, 1));
	
	// Motor
	_motor_relative_pos = Vector3D(-BODY_SIZE.x, 0, 0);
	_motor_initial_quaternion = physx::PxQuat(physx::PxPi, Vector3(0, 1, 0));

	_propellers = new SubmarinePropeller(8, _gPhysics);
	_propellers->core_piece()->setQuaternion(_motor_initial_quaternion); // Giramos 180º
	_propellers->core_piece()->setPosition(position + _motor_relative_pos); // Las helices van en la cola del submarino

	_motor_spin_phase_quaternion = physx::PxQuat(physx::PxIdentity);

	_propellers_inv_inertia_diagonal = _propellers->getInvInerceTensorDiagonal();

	// Timón
	_rudder_relative_pos = Vector3D(-BODY_SIZE.x, 0, 0);
	_rudder_initial_quaternion = physx::PxQuat(physx::PxPi * 0.25, Vector3(0, 1, 0)) * physx::PxQuat(physx::PxPi * -0.5, Vector3(1, 0, 0)) * physx::PxQuat(physx::PxPi * 0.5, Vector3(0, 1, 0));

	_rudder = new PropellerBladePiece(position + _rudder_relative_pos, 9, 4, 1, 5, {0, 1, 1, 1});
	_rudder->reaction_mode = PropellerBladePiece::LINEAR;
	_rudder->setQuaternion(_rudder_initial_quaternion);

	// Tensor de inercia
	setMassSpaceInvInertiaTensor();

	// Posición de la cámara en primera persona
	_subarine_eye = Vector3D(BODY_SIZE.x + 1, 0, 0);

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
	delete _center_mass;
}

float Submarine::getDensity() 
{
	if (!_center_mass) return 0;
	return _center_mass->mass() / _center_mass->volume();
}

void Submarine::setMassSpaceInvInertiaTensor()
{
	physx::PxRigidDynamic* temp_submarine_body = _gPhysics->createRigidDynamic(physx::PxTransform({0,0,0}));

	// Definir forma del submarino
	physx::PxShape* body_shape = CreateShape(physx::PxBoxGeometry(BODY_SIZE.x, BODY_SIZE.y, BODY_SIZE.z));
	body_shape->setLocalPose(physx::PxTransform({ 0,0,0 }));

	temp_submarine_body->attachShape(*body_shape);

	// Obtenemos el tensor de inercia
	physx::PxRigidBodyExt::updateMassAndInertia(*temp_submarine_body, getDensity());

	_submarine_inv_inertia_diagonal = temp_submarine_body->getMassSpaceInvInertiaTensor();
}

void Submarine::update(float t)
{
	// Control de cámara
	handleCameraFollow();
	
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
	// [1] OBTENCIÓN DE FUERZAS

	Vector3D motor_torque = _propellers->core_piece()->transform().q.rotate(_propellers->base_orientation.to_vec3()) * 1000000;//Vector3D(-10000, 0, 0);

	// Aplicación de las fuerzas y obtención de reacciones

	auto motor_reaction_forces = _propellers->propagateForces({ Vector3D(), motor_torque, Vector3D(), _propellers->core_piece()->transform().p });
	
	// Es una sola pieza, no hace falta propagar fuerzas, solo obtener la reacción de la propia pieza
	auto rudder_reaction_forces = _rudder->applyPieceReactionForces({ _center_mass->velocity(), Vector3D(), _rudder->transform().p, Vector3D()});

	// Cálculo de las fuerzas totales

	Vector3D total_torque = (motor_torque + motor_reaction_forces.torque);
	Vector3D total_linaer_force = motor_reaction_forces.force + rudder_reaction_forces.force;


	// [2] CÁLCULO DE ACELERACIÓN

	// Aceleración angular del submarino entero
	Vector3D rudder_torque = rudder_reaction_forces.force.cross(_rudder_relative_pos);
	rudder_torque.x = rudder_torque.z = 0; // Limpiando imperfecciones en el torque

	// Vector3D subamrine_angular_acceleration = Vector3D(0, 0.001, 0);// rudder_torque.normalized()* t;
	// Aceleración angular de las hélices
	Vector3D subamrine_angular_acceleration = Vector3D(
		_submarine_inv_inertia_diagonal.x * rudder_torque.x,
		_submarine_inv_inertia_diagonal.y * rudder_torque.y,
		_submarine_inv_inertia_diagonal.z * rudder_torque.z
	);

	// Aceleración angular de las hélices
	Vector3D angular_acceleration = Vector3D(
		_propellers_inv_inertia_diagonal.x * total_torque.x,
		_propellers_inv_inertia_diagonal.y * total_torque.y,
		_propellers_inv_inertia_diagonal.z * total_torque.z
	);

	Vector3D new_angular_velocity = _propellers->core_piece()->angular_velocity() + angular_acceleration;

	// Truncando el ángulo aplicado para evitar que se rompa el programa y aplicando el tiempoi delta
	new_angular_velocity = new_angular_velocity.normalized() * fmod(new_angular_velocity.magnitude(), (2 * physx::PxPi)) * t;
	std::cout << "Old Spin " << subamrine_angular_acceleration.to_str() << "\n";
	subamrine_angular_acceleration = subamrine_angular_acceleration.normalized() * fmod(subamrine_angular_acceleration.magnitude(), (2 * physx::PxPi)) * t;

	// [3] APLICANDO MOVIMIENTO ANGULAR (Rotación del submarino por el timón)

	// Rotación del motor respecto su (1, 0, 0) local
	physx::PxQuat motor_rotation = physx::PxQuat(new_angular_velocity.magnitude(), Vector3D(1, 0, 0).to_vec3());

	// Guardando la rotación locar de la hélice sobre su eje de giro
	_motor_spin_phase_quaternion = motor_rotation * _motor_spin_phase_quaternion;

	physx::PxQuat submarine_rotation = physx::PxQuat(physx::PxIdentity);

	if (subamrine_angular_acceleration.magnitude() > 0)
		submarine_rotation = physx::PxQuat(subamrine_angular_acceleration.magnitude(), subamrine_angular_acceleration.normalized().to_vec3());

	// Rotando el cuerpo del submarino
	_center_mass->transform().q = submarine_rotation * _center_mass->transform().q;

	// Rotando las piezas del submarino (la posición se actualiza luego)
	_propellers->core_piece()->setQuaternion(_center_mass->transform().q * _motor_initial_quaternion * _motor_spin_phase_quaternion);
	_rudder->setQuaternion(_center_mass->transform().q * _rudder_initial_quaternion);
	

	// [4] APLICANDO MOVIMIENTO LINEAL

	// Acelerando el cuerpo del submarino
	_center_mass->acceleration() += total_linaer_force / (_center_mass->mass() + _propellers->mass());
	
	// Moviendo las piezas unidas al cuerpo del submarino y desplazándolas linealmente

	_propellers->core_piece()->propagateMotionEffect({
		_propellers->core_piece()->transform().p, // Centro del movimiento
		(_center_mass->transform().q.rotate(_motor_relative_pos.to_vec3()) + _center_mass->position()) - _propellers->core_piece()->transform().p, // Desplazamiento lineal
		new_angular_velocity // Rotación
		});

	_rudder->propagateMotionEffect({
		_rudder->transform().p, // Centro del movimiento
		(_center_mass->transform().q.rotate(_rudder_relative_pos.to_vec3()) + _center_mass->position()) - _rudder->transform().p, // Desplazamiento lineal
		Vector3D()// Rotación
		});
	
	std::cout << "Pos " << Vector3D(_center_mass->position()).to_str() << "\n";
	std::cout << "Spin " << subamrine_angular_acceleration.to_str() << "\n\n";

	// Generando partículas de burbuja

	if (_motor_force < 100)
		_motor_particle_generator->setGenerationPeriod(-1);
	else
		_motor_particle_generator->setGenerationPeriod(0.15);
}
