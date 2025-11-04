#pragma once
#include "../../../RenderUtils.hpp"
#include "../../../Utils/Vector3D.h"

class IndustrialPiece : public RenderItem
{
public:

	struct AttachmentPoint
	{
		IndustrialPiece* industrial_piece;
		AttachmentPoint* connected_point;
		Vector3D relative_position; // Posición desde el centro de la pieza (sin rotar)
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

	struct MotionTransmitionPack
	{
		Vector3D motion_center;
		Vector3D translation;
		physx::PxQuat rotation;
	};

	IndustrialPiece(Vector3D position, float mass, const Vector4& _color = Vector4(1, 1, 1, 1));

	/*
		Propaga hacia delante(sus componentes conexos) un paquete de fuerzas (torque y fuerza lineal) y devuelve 
		la fuerza reactiva resultante de la aplicación de la fuerza en los componentes conexos derivados.

		AttachmentPoint se utiliza para no propagar la fuerza hacia delante al mismo AttachmentPoint que
		está transmitiendo la fuerza, solo después de calcular la reacción se le devolverá fuerza a este
		a través del return. 
	*/
	ForceTransmisionPack propagateForces(const ForceTransmisionPack& force_pack, const AttachmentPoint& force_emitter_point);

	virtual ForceTransmisionPack applyPieceReactionForces(const ForceTransmisionPack& force_pack) {
		return { Vector3D(), Vector3D(), Vector3D(), Vector3D() };
	}

	void propagateMotionEffect(MotionTransmitionPack motion);

protected:

	physx::PxTransform _transform;
	float _mass;

	Vector3D _position_to_machine_center; // Desde el centro de la pieza hasta el centro de masas de la máquina
	physx::PxQuat _orientation_to_machine; // Desde la orientación de la pieza hasta la orientación de la máquina

	std::vector<AttachmentPoint> _attachment_points;

	bool _reaction_effect_applied = false; // Usado en propagateEffect() para aplicar los cambios una sola vez
};