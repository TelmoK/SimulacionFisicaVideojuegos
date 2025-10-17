#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class IndustrialPiece : public RenderItem
{
public:

	struct AttachmentPoint
	{
		IndustrialPiece* industrial_piece;
		AttachmentPoint* connected_point;
	};

	struct ForceTransmisionPack
	{
		Vector3D force;
		Vector3D torque;
		Vector3D torque_axis_point;
	};

	IndustrialPiece(physx::PxShape* _shape, float mass, const Vector4& _color = Vector4(1, 1, 1, 1));

	/*
		Propaga hacia delante(sus componentes conexos) un paquete de fuerzas (torque y fuerza lineal) y devuelve 
		la fuerza reactiva resultante de la aplicación de la fuerza en los componentes conexos derivados.

		AttachmentPoint se utiliza para no propagar la fuerza hacia delante al mismo AttachmentPoint que
		está transmitiendo la fuerza, solo después de calcular la reacción se le devolverá fuerza a este
		a través del return. 
	*/
	virtual ForceTransmisionPack propagateForces(AttachmentPoint* force_emitter_point);

protected:

	float _mass;
	std::vector<AttachmentPoint*> _attachment_points;
};