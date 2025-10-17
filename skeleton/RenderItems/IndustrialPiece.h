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
		la fuerza reactiva resultante de la aplicaci�n de la fuerza en los componentes conexos derivados.

		AttachmentPoint se utiliza para no propagar la fuerza hacia delante al mismo AttachmentPoint que
		est� transmitiendo la fuerza, solo despu�s de calcular la reacci�n se le devolver� fuerza a este
		a trav�s del return. 
	*/
	virtual ForceTransmisionPack propagateForces(AttachmentPoint* force_emitter_point);

protected:

	float _mass;
	std::vector<AttachmentPoint*> _attachment_points;
};