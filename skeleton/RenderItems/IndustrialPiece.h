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
		Vector3D relative_position; // Posición desde el centro de la pieza
	};

	struct ForceTransmisionPack
	{
		Vector3D force;
		Vector3D torque;
		Vector3D force_axis_point; // Necesario si la pieza madre tiene efecto de torsión sobre su centro de masas
		Vector3D torque_axis_point;

		ForceTransmisionPack operator+(const ForceTransmisionPack& force_pack)
		{
			return {
				force + force_pack.force, 
				torque + force_pack.torque,
				(force_axis_point + force_pack.force_axis_point) / 2,
				(torque_axis_point + force_pack.torque_axis_point) / 2
			};
		}

		void operator+=(const ForceTransmisionPack& force_pack)
		{
			force = force + force_pack.force;
			torque = torque + force_pack.torque;
			force_axis_point = (force_axis_point + force_pack.force_axis_point) / 2;
			torque_axis_point = (torque_axis_point + force_pack.torque_axis_point) / 2;
		}
	};


	IndustrialPiece(Vector3D position, float mass, const Vector4& _color = Vector4(1, 1, 1, 1));

	/*
		Propaga hacia delante(sus componentes conexos) un paquete de fuerzas (torque y fuerza lineal) y devuelve 
		la fuerza reactiva resultante de la aplicación de la fuerza en los componentes conexos derivados.

		AttachmentPoint se utiliza para no propagar la fuerza hacia delante al mismo AttachmentPoint que
		está transmitiendo la fuerza, solo después de calcular la reacción se le devolverá fuerza a este
		a través del return. 
	*/
	virtual ForceTransmisionPack propagateForces(ForceTransmisionPack force_pack, AttachmentPoint force_emitter_point);

	void propagateEffect(std::pair<physx::PxVec3, physx::PxQuat> translation_rotation_pair);

protected:

	physx::PxTransform _transform;
	float _mass;
	std::vector<AttachmentPoint> _attachment_points;
};