#include "IndustrialPiece.h"

IndustrialPiece::IndustrialPiece(Vector3D position, float mass, const Vector4& _color)
	: RenderItem(CreateShape(physx::PxBoxGeometry(1, 0.5, 2)), &_transform, _color)
{
	_transform = physx::PxTransform(position.to_vec3());
}

IndustrialPiece::ForceTransmisionPack IndustrialPiece::propagateForces(const ForceTransmisionPack& force_pack, const AttachmentPoint& force_emitter_point)
{
	ForceTransmisionPack sum_force_pack{ Vector3D(), Vector3D(), Vector3D(), Vector3D() };

	for (AttachmentPoint attachment_point : _attachment_points)
	{
		if (attachment_point.industrial_piece != force_emitter_point.industrial_piece)
		{
			sum_force_pack += attachment_point.connected_point->industrial_piece->propagateForces(force_pack, attachment_point);
		}
	}

	// Aquí se aplica alguna transformación a force_pack en función de la pieza actual
	sum_force_pack += applyPieceReactionForces(force_pack);

	return sum_force_pack;
}

void IndustrialPiece::propagateEffect(std::pair<physx::PxVec3, physx::PxQuat> translation_rotation_pair)
{
	_reaction_effect_applied = true;

	// Se aplica la transformación
	_transform.p += translation_rotation_pair.first;
	_transform.q += translation_rotation_pair.second;

	// Se transmite la orden de update a las piezas conectadas que no se hayan actualizado ya
	for(AttachmentPoint attachment_point : _attachment_points)
		if (!attachment_point.industrial_piece->_reaction_effect_applied)
			attachment_point.industrial_piece->propagateEffect(translation_rotation_pair);

	// Cuando la propagación ha acabado se vuleve a establecer la pieza como no actualizada
	_reaction_effect_applied = false;
}
