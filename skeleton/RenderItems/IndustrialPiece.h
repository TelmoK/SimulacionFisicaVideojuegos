#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class IndustrialPiece : public RenderItem
{
public:
	IndustrialPiece(physx::PxShape* _shape, float mass, const Vector4& _color = Vector4(1, 1, 1, 1));

	virtual Vector3D reiciveLinearAcceleration();
	virtual Vector3D reiciveAngularAcceleration();

protected:

	float _mass;
	std::vector<Vector3D> _attachment_points;
};