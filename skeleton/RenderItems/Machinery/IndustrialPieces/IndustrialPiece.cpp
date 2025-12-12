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

	physx::PxQuat rotation = physx::PxQuat(0, Vector3(1, 0, 0));

	Vector3 ang_vel = motion.angular_velocity.normalized().to_vec3();
	float ang_vel_mag = motion.angular_velocity.magnitude();
	// Evitamos la creación de un quaternion inválido
	if (ang_vel_mag > 0)
		rotation = physx::PxQuat(motion.angular_velocity.magnitude(), ang_vel);

	// Se aplica la transformación angular
	Vector3D positionToCenter =  motion.motion_center - _transform.p;
	Vector3D rotatedPositionToCenter = rotation.rotate(positionToCenter.to_vec3());

	_transform.p = (motion.motion_center - rotatedPositionToCenter).to_vec3(); // Aplicar movimiento circular sobre el centro de movimiento

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

void IndustrialPiece::setQuaternion(physx::PxQuat q)
{
	Vector3D current_rotation_vec = Vector3D(_transform.q.x, _transform.q.y, _transform.q.z).normalized() * physx::PxAcos(_transform.q.w) * 2;
	Vector3D rotation_vec = Vector3D(q.x, q.y, q.z).normalized() * physx::PxAcos(q.w) * 2;

	// Guardar las velocidades actuales de la pieza (linear y angular)
	Vector3D current_linear_vel = _linear_velocity;
	Vector3D current_angular_vel = _angular_velocity;

	// Deshacer la rotación global que tiene la pieza
	propagateMotionEffect({ _transform.p, Vector3D(), -current_rotation_vec });

	// Aplicar la rotación deseada
	propagateMotionEffect({ _transform.p, Vector3D(), rotation_vec});

	// Aplicar desplazamiento nulo para setear las velocidades a cero
	propagateMotionEffect({ _transform.p, Vector3D(), Vector3D() });

	// Volver a settear las velocidades que tenía la pieza
	_linear_velocity = current_linear_vel;
	_angular_velocity = current_angular_vel;
}

void IndustrialPiece::setPosition(Vector3D p)
{
	Vector3D translation = p - _transform.p;

	// Guardar las velocidades actuales de la pieza (linear y angular)
	Vector3D current_linear_vel = _linear_velocity;
	Vector3D current_angular_vel = _angular_velocity;

	// Aplicar la traslación deseada
	propagateMotionEffect({ _transform.p, translation, Vector3D() });

	// Aplicar desplazamiento nulo para setear las velocidades a cero
	propagateMotionEffect({ _transform.p, Vector3D(), Vector3D() });

	// Volver a settear las velocidades que tenía la pieza
	_linear_velocity = current_linear_vel;
	_angular_velocity = current_angular_vel;
}
