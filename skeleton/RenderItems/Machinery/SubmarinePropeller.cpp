#include "SubmarinePropeller.h"
#include <iostream>

SubmarinePropeller::SubmarinePropeller(int blade_num, physx::PxPhysics* gPhysics) 
	: _blade_num(blade_num), _gPhysics(gPhysics)
{
	float volume = 0;

	// Creando el Eje de la hélice
	_shaft_local_transform.p = Vector3D(SHAFT_SIZE.x, 0, 0).to_vec3();
	_shaft_local_transform.q.normalize();

	_shaft = new IndustrialPiece(
			CreateShape(physx::PxBoxGeometry(SHAFT_SIZE.x, SHAFT_SIZE.y, SHAFT_SIZE.z)),
			_shaft_local_transform.p,
			10, 
			Vector4(1, 0, 1, 1)
		);

	// Punto de anclaje del eje
	_shaft_ap = new IndustrialPiece::AttachmentPoint{ _shaft, nullptr, Vector3D(0, 0, 1) };
	_shaft->addAttachmentPoint(_shaft_ap);


	// Creando el Núcleo de la hélice
	_boss_local_transform.p = Vector3D(SHAFT_SIZE.x * 2 + BOSS_SIZE.x, 0, 0).to_vec3();
	_boss_local_transform.q.normalize();

	volume += SHAFT_SIZE.x * SHAFT_SIZE.y * SHAFT_SIZE.z; // Sumamos al volumen total de las hélices

	_boss = new IndustrialPiece(
			CreateShape(physx::PxBoxGeometry(BOSS_SIZE.x, BOSS_SIZE.y, BOSS_SIZE.z)),
			_boss_local_transform.p,
			10,
			Vector4(1, 0, 0, 1)
		);

	// Punto de anclaje del núcleo
	auto boss_shaft_ap = new IndustrialPiece::AttachmentPoint{ _boss, nullptr, Vector3D(0, 0, -1) };
	_boss->addAttachmentPoint(boss_shaft_ap);

	volume += BOSS_SIZE.x * BOSS_SIZE.y * BOSS_SIZE.z; // Sumamos al volumen total de las hélices

	_shaft_ap->linkTo(boss_shaft_ap); // Juntando el eje con el núcleo

	// Hélices
	_blade_angle = physx::PxPi / 6;

	for (int i = 0; i < blade_num; ++i)
	{
		PropellerBladePiece* blade = new PropellerBladePiece(
			Vector3D(SHAFT_SIZE.x, 0, 0),
			BLADE_SIZE.x, BLADE_SIZE.z, BLADE_SIZE.y,
			10,
			Vector4(1, 0, 1, 1)
		);

		blade->reaction_mode = PropellerBladePiece::ANGULAR;

		// Posicionamiento y Orientación de la pala
		physx::PxQuat rotation_parallel_to_boss = physx::PxQuat(physx::PxPi / 2, Vector3D(0, 0, 1).to_vec3());
		physx::PxQuat rotation_in_boss = physx::PxQuat(_blade_angle, Vector3D(0, 1, 0).to_vec3());
		physx::PxQuat rotation_in_shaft = physx::PxQuat((physx::PxPi * 2 * (i+1)) / blade_num, Vector3D(1, 0, 0).to_vec3());
		blade->transform().q = rotation_in_shaft * rotation_in_boss * rotation_parallel_to_boss;
		blade->transform().q.normalize();

		blade->transform().p = blade->transform().q.rotate(Vector3(1, 0, 0) * (BOSS_SIZE.y*2));
		blade->transform().p += Vector3D(SHAFT_SIZE.x * 2 + BOSS_SIZE.x, 0, 0).to_vec3();

		_blades_local_transform.push_back(blade->transform()); // Añadimos la posición local para manejar el tensor de inercia

		volume += BLADE_SIZE.x * BLADE_SIZE.y * BLADE_SIZE.z; // Sumamos al volumen total de las hélices

		// Juntando la pala con el núcleo
		auto blade_ap = new IndustrialPiece::AttachmentPoint{ blade, nullptr, Vector3D(0, 0, 1) };
		blade->addAttachmentPoint(blade_ap);

		auto boss_blade_ap = new IndustrialPiece::AttachmentPoint{ _boss, nullptr, Vector3D(0, 0, -1) };
		_boss->addAttachmentPoint(boss_blade_ap);

		_blades.push_back(blade);
		_attachment_points.push_back(blade_ap);

		boss_blade_ap->linkTo(blade_ap);
	}

	// Setteando la masa y la densidad del conjunto

	_mass = SHAFT_MASS + BOSS_MASS + BLADE_MASS * blade_num;
	_density = _mass / volume;
}

SubmarinePropeller::~SubmarinePropeller()
{
	delete _shaft;
	delete _boss;

	for (PropellerBladePiece* blade : _blades)
		delete blade;

	for (auto* ap : _attachment_points)
		delete ap;
}

Vector3D SubmarinePropeller::getInvInerceTensorDiagonal()
{
	physx::PxRigidDynamic* temp_propeller_body = _gPhysics->createRigidDynamic(physx::PxTransform({0,0,0}));

	// Añadir la forma del eje
	physx::PxShape* shaftShape = CreateShape(physx::PxBoxGeometry(SHAFT_SIZE.x, SHAFT_SIZE.y, SHAFT_SIZE.z));
	shaftShape->setLocalPose(_shaft_local_transform);

	temp_propeller_body->attachShape(*shaftShape);

	// Añadir la forma del núcleo
	physx::PxShape* bossShape = CreateShape(physx::PxBoxGeometry(BOSS_SIZE.x, BOSS_SIZE.y, BOSS_SIZE.z));
	bossShape->setLocalPose(_boss_local_transform);

	temp_propeller_body->attachShape(*bossShape);

	// Añadir la forma de las palas
	for (physx::PxTransform blade_local_tr : _blades_local_transform)
	{
		physx::PxShape* bladeShape = CreateShape(physx::PxBoxGeometry(BLADE_SIZE.x, BLADE_SIZE.y, BLADE_SIZE.z));
		bladeShape->setLocalPose(blade_local_tr);

		temp_propeller_body->attachShape(*bladeShape);
	}

	// Obtenemos el tensor de inercia de la pieza compuesta
	physx::PxRigidBodyExt::updateMassAndInertia(*temp_propeller_body, _density);

	return temp_propeller_body->getMassSpaceInvInertiaTensor();
}

IndustrialPiece::ForceTransmisionPack SubmarinePropeller::propagateForces(const IndustrialPiece::ForceTransmisionPack& force_pack)
{
	return _shaft->propagateForces(force_pack, _shaft_ap);
}

void SubmarinePropeller::update(double t)
{
/*

	EJEMPLO DE MOVIMIENTO DE LAS HÉLICES DE FORMA INTEPENDIENTE
	
	// El torque debe seguir la orientación de las hélices.
	// Cuidado con los valores muy altos.
	Vector3D torque = Vector3D(1000,0, 0); 

	// Aplicación de las fuerzas y obtención de reacciones
	IndustrialPiece::ForceTransmisionPack motor_forces{ Vector3D(), torque, Vector3D(), _shaft->transform().p };
	auto reaction_forces = _shaft->propagateForces(motor_forces, _shaft_ap);

	// Cálculo de las fuerzas totales
	Vector3D total_torque = (motor_forces.torque + reaction_forces.torque);
	Vector3D total_linaer_force = (motor_forces.force + reaction_forces.force);

	// Cálulo de las aceleraciones

	// Las rotaciones se aplica en los ejes principales de las hélices, por lo que nos basta con la diagonal
	// del tensor de inercia inverso
	Vector3D inv_inertia_diagonal = getInvInerceTensorDiagonal();

	// Aceleración angular
	Vector3D angular_acceleration = Vector3D(
		inv_inertia_diagonal.x * total_torque.x,
		inv_inertia_diagonal.y * total_torque.y,
		inv_inertia_diagonal.z * total_torque.z
	);

	// Aceleración lineal
	Vector3D linear_acceleration = total_linaer_force / _mass;
	
	// Aplicando aceleración
	Vector3D new_linear_velocity = _shaft->linear_velocity() + linear_acceleration;
	Vector3D new_angular_velocity = _shaft->angular_velocity() + angular_acceleration;

	// Moviendo la pieza
	Vector3D p = Vector3D(_shaft->transform().p);
	_shaft->propagateMotionEffect({ p, new_linear_velocity * t, new_angular_velocity * t});

	// DEBUG DE LOS VALORES DE MOVIMIENTO
	// std::cout << "Velocidad Lineal:  " << (new_linear_velocity * t).to_str() << "\n";
	// std::cout << "Velocidad Angular: " << (new_angular_velocity * t).to_str() << "\n\n";

*/
}

