#include "IndustrialPiece.h"

IndustrialPiece::IndustrialPiece(physx::PxShape* _shape, float mass, const Vector4& _color)
	: RenderItem()
{

}

IndustrialPiece::ForceTransmisionPack IndustrialPiece::propagateForces(ForceTransmisionPack force_pack, AttachmentPoint force_emitter_point)
{
	ForceTransmisionPack sum_force_pack{ Vector3D(), Vector3D(), Vector3D(), Vector3D() };

	for (AttachmentPoint attachment_point : _attachment_points)
	{
		if (attachment_point.industrial_piece != force_emitter_point.industrial_piece)
		{
			sum_force_pack += attachment_point.connected_point->industrial_piece->propagateForces(force_pack, attachment_point);
		}
	}

	return sum_force_pack;
}

void IndustrialPiece::propagateEffect(std::pair<physx::PxVec3, physx::PxQuat> translation_rotation_pair)
{

}
