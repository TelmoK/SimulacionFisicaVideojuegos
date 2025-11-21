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
}

SubmarinePropeller::~SubmarinePropeller()
{
	delete _shaft;
	delete _boss;

	for (PropellerBladePiece* blade : _blades)
		delete blade;
}

