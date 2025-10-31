#include "IndustrialPiece.h"

IndustrialPiece::IndustrialPiece(Vector3D position, float mass, const Vector4& _color)
	: RenderItem(CreateShape(physx::PxBoxGeometry(1, 0.5, 2)), &_transform, _color)
{
	_transform = physx::PxTransform(position.to_vec3());
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

	// Aqu� se podr�a aplicar alguna transformaci�n a force_pack en funci�n de la pieza actual
	// sum_force_pack += aplyPieceReactionForces(force_pack);

	return sum_force_pack;
}

void IndustrialPiece::propagateEffect(std::pair<physx::PxVec3, physx::PxQuat> translation_rotation_pair)
{
	_transform.p += translation_rotation_pair.first;
}
