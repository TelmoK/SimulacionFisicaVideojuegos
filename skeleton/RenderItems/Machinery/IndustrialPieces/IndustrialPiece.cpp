#include "IndustrialPiece.h"

IndustrialPiece::IndustrialPiece(physx::PxShape* shape, Vector3D position, float mass, const Vector4& _color)
	: RenderItem(shape, &_transform, _color), _surface_normal(Vector3D(0, 1, 0))
{
	_transform = physx::PxTransform(position.to_vec3());
}

IndustrialPiece::IndustrialPiece(Vector3D position, float mass, const Vector4& _color)
	: IndustrialPiece(CreateShape(physx::PxBoxGeometry(1, 1, 1)), position, mass, _color)
{
	
}

IndustrialPiece::ForceTransmisionPack IndustrialPiece::propagateForces(const ForceTransmisionPack& force_pack, AttachmentPoint* force_emitter_point)
{
	ForceTransmisionPack sum_force_pack{ Vector3D(), Vector3D(), Vector3D(), Vector3D() };

	for (AttachmentPoint* attachment_point : _attachment_points)
	{
		if (attachment_point->connected_point->industrial_piece != force_emitter_point->industrial_piece)
		{
			sum_force_pack += attachment_point->connected_point->industrial_piece->propagateForces(force_pack, attachment_point);
		}
	}

	// Aquí se aplica alguna transformación a force_pack en función de la pieza actual
	sum_force_pack += applyPieceReactionForces(force_pack, force_emitter_point);

	return sum_force_pack;
}

void IndustrialPiece::propagateMotionEffect(MotionTransmitionPack motion)
{
	_reaction_effect_applied = true;

	_linear_velocity = motion.linear_velocity;
	_angular_velocity = motion.angular_velocity;

	Vector3D translation = _linear_velocity;
	physx::PxQuat rotation = physx::PxQuat(motion.angular_velocity.magnitude(), motion.angular_velocity.normalized().to_vec3());

	// Se aplica la transformación angular
	Vector3D positionToCenter = motion.motion_center - _transform.p;
	Vector3D rotatedPositionToCenter = rotation.rotate(positionToCenter.to_vec3());

	_transform.p += (motion.motion_center - rotatedPositionToCenter).to_vec3(); // Aplicar movimiento circular sobre el centro de movimiento

	_transform.q = rotation * _transform.q;

	// Se aplica la transformación linear
	_transform.p += translation.to_vec3();

	// Se transmite la orden de update a las piezas conectadas que no se hayan actualizado ya
	for(AttachmentPoint* attachment_point : _attachment_points)
		if (!attachment_point->connected_point->industrial_piece->_reaction_effect_applied)
			attachment_point->connected_point->industrial_piece->propagateMotionEffect(motion);

	// Cuando la propagación ha acabado se vuleve a establecer la pieza como no actualizada
	_reaction_effect_applied = false;
}
