#include "PropellerBladePiece.h"

#include "../../../Utils/SpaceGeometry.h"

PropellerBladePiece::PropellerBladePiece(Vector3D position, float lenght, float width, float thickness, float mass, const Vector4& _color)
	: IndustrialPiece(CreateShape(physx::PxBoxGeometry(lenght, thickness, width)), position, mass, _color)
{
	_surface_area = lenght * width;
}

IndustrialPiece::ForceTransmisionPack PropellerBladePiece::applyPieceReactionForces(const ForceTransmisionPack& force_pack, AttachmentPoint* force_emitter_point)
{
	// La posición global del punto del AttachmentPoint que recibe el torque
	Vector3D attachment_global_pos = _transform.p + force_emitter_point->connected_point->relative_position.to_vec3();
	
	Vector3D radial_position = SpaceGeometry::distanceLineToPoint(force_pack.torque_axis_point, force_pack.torque, attachment_global_pos);

	float force_magnitude = force_pack.torque.magnitude() / radial_position.magnitude();

	Vector3D linear_force = radial_position.cross(force_pack.torque) * force_magnitude;

	// Cálculo de la racción

	Vector3D linear_velocity = linear_force; // TODO: La fuerza no es directamente la velocidad

	Vector3D blade_normal_in_world = _transform.q.rotate(_surface_normal.to_vec3());

	float cos_velocity_surface = SpaceGeometry::cosineBetween(linear_velocity, blade_normal_in_world);

	float DRAG_COEF = _base_drag_coef + (_max_drag_coef - _base_drag_coef) * cos_velocity_surface;


	// TODO Se usa la velocidad angular que está almacenada el la pieza pero que proviene de la máquina
	Vector3D blade_velocity = _angular_velocity * radial_position.magnitude();

	Vector3D drag_force = blade_velocity * -0.5 * WATER_DENSITY * pow(_linear_velocity.magnitude(), 2)
		* _surface_area * DRAG_COEF;

	//Vector3D lift_force = 

	return ForceTransmisionPack();
}
