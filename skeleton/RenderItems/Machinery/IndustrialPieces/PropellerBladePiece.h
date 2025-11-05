#pragma once
#include "IndustrialPiece.h"

class PropellerBladePiece : public IndustrialPiece
{
public:

	PropellerBladePiece(Vector3D position, float lenght = 4, float width = 2, float thickness = 1, float mass = 1, const Vector4& _color = Vector4(1, 1, 1, 1));

	ForceTransmisionPack applyPieceReactionForces(const ForceTransmisionPack& force_pack, AttachmentPoint* force_emitter_point) override;

protected:

	float _surface_area = 0;

	float _base_drag_coef = 0.1;
	float _max_drag_coef = 0.3;

	const float WATER_DENSITY = 997;
};