#include "SumarinePropeller.h"

SubmarinePropeller::SubmarinePropeller(int blade_num) : _blade_num(blade_num)
{
	_shaft = new IndustrialPiece(
			CreateShape(physx::PxBoxGeometry(SHAFT_SIZE.x, SHAFT_SIZE.y, SHAFT_SIZE.z)),
			Vector3D(SHAFT_SIZE.x, 0, 0),
			10, 
			Vector4(1, 0, 1, 1)
		);

	auto shaft_ap = new IndustrialPiece::AttachmentPoint{ _shaft, nullptr, Vector3D(0, 0, 1) };
	_shaft->addAttachmentPoint(shaft_ap);

	_boss = new IndustrialPiece(
			CreateShape(physx::PxBoxGeometry(BOSS_SIZE.x, BOSS_SIZE.y, BOSS_SIZE.z)),
			Vector3D(SHAFT_SIZE.x * 2 + BOSS_SIZE.x, 0, 0),
			10,
			Vector4(1, 0, 0, 1)
		);

	auto boss_shaft_ap = new IndustrialPiece::AttachmentPoint{ _boss, nullptr, Vector3D(0, 0, -1) };
	_boss->addAttachmentPoint(boss_shaft_ap);

	shaft_ap->linkTo(boss_shaft_ap); // Juntando el eje con el núcleo

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

		blade->transform().p = blade->transform().q.rotate(Vector3(1, 0, 0) * (BOSS_SIZE.y*2));
		blade->transform().p += Vector3D(SHAFT_SIZE.x * 2 + BOSS_SIZE.x, 0, 0).to_vec3();

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

void SubmarinePropeller::update(double t)
{
	_shaft->propagateMotionEffect({ Vector3D(_shaft->transform().p), Vector3D(0, 1, 0) * t, Vector3D(1, 0, 0) * t });
}

