#include "SumarinePropeller.h"
#include <iostream>

SubmarinePropeller::SubmarinePropeller(int blade_num, physx::PxPhysics* gPhysics) : _blade_num(blade_num), _gPhysics(gPhysics)
{
	// Creando el Eje de la hélice
	_shaft_local_transform.p = Vector3D(SHAFT_SIZE.x, 0, 0).to_vec3();
	_shaft_local_transform.q.normalize();

	_shaft = new IndustrialPiece(
			CreateShape(physx::PxBoxGeometry(SHAFT_SIZE.x, SHAFT_SIZE.y, SHAFT_SIZE.z)),
			_shaft_local_transform.p,
			10, 
			Vector4(1, 0, 1, 1)
		);

	_shaft_ap = new IndustrialPiece::AttachmentPoint{ _shaft, nullptr, Vector3D(0, 0, 1) };
	_shaft->addAttachmentPoint(_shaft_ap);

	// Creando el Núcleo de la hélice
	_boss_local_transform.p = Vector3D(SHAFT_SIZE.x * 2 + BOSS_SIZE.x, 0, 0).to_vec3();
	_boss_local_transform.q.normalize();

	_boss = new IndustrialPiece(
			CreateShape(physx::PxBoxGeometry(BOSS_SIZE.x, BOSS_SIZE.y, BOSS_SIZE.z)),
			_boss_local_transform.p,
			10,
			Vector4(1, 0, 0, 1)
		);

	auto boss_shaft_ap = new IndustrialPiece::AttachmentPoint{ _boss, nullptr, Vector3D(0, 0, -1) };
	_boss->addAttachmentPoint(boss_shaft_ap);

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

		physx::PxQuat rotation_parallel_to_boss = physx::PxQuat(physx::PxPi / 2, Vector3D(0, 0, 1).to_vec3());
		physx::PxQuat rotation_in_boss = physx::PxQuat(_blade_angle*0, Vector3D(0, 1, 0).to_vec3());
		physx::PxQuat rotation_in_shaft = physx::PxQuat((physx::PxPi * 2 * (i+1)) / blade_num, Vector3D(1, 0, 0).to_vec3());
		blade->transform().q = rotation_in_shaft * rotation_in_boss * rotation_parallel_to_boss;
		blade->transform().q.normalize();

		blade->transform().p = blade->transform().q.rotate(Vector3(1, 0, 0) * (BOSS_SIZE.y*2));
		blade->transform().p += Vector3D(SHAFT_SIZE.x * 2 + BOSS_SIZE.x, 0, 0).to_vec3();

		_blades_local_transform.push_back(blade->transform()); // Añadimos la posición local para manejar el tensor de inercia

		// Juntando la pala con el núcleo
		auto blade_ap = new IndustrialPiece::AttachmentPoint{ blade, nullptr, Vector3D(0, 0, 1) };
		blade->addAttachmentPoint(blade_ap);

		auto boss_blade_ap = new IndustrialPiece::AttachmentPoint{ _boss, nullptr, Vector3D(0, 0, -1) };
		_boss->addAttachmentPoint(boss_blade_ap);

		_blades.push_back(blade);
		_attachment_points.push_back(blade_ap);

		boss_blade_ap->linkTo(blade_ap);
	}
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


	physx::PxRigidBodyExt::updateMassAndInertia(*temp_propeller_body, DENSITY);

	return temp_propeller_body->getMassSpaceInvInertiaTensor();
}

void SubmarinePropeller::update(double t)
{
	IndustrialPiece::ForceTransmisionPack motor_forces{ Vector3D(), Vector3D(1, 0, 0) * t, Vector3D(), _shaft->transform().p };
	auto reaction_forces = _shaft->propagateForces(motor_forces, _shaft_ap);

	Vector3D angular_acceleration = (motor_forces.torque - reaction_forces.torque);
	Vector3D linear_acceleration = (motor_forces.force - reaction_forces.force);

	_shaft->propagateMotionEffect({ Vector3D(_shaft->transform().p), linear_acceleration.normalized()*0 * t, angular_acceleration.normalized()*0 * t });
	//_shaft->propagateMotionEffect({ Vector3D(_shaft->transform().p), Vector3D(1, 0, 0) * t, Vector3D(1, 0, 0) * t });
}

