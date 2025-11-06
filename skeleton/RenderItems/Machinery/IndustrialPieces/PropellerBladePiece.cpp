#include "PropellerBladePiece.h"

#include "../../../Utils/SpaceGeometry.h"

PropellerBladePiece::PropellerBladePiece(Vector3D position, float lenght, float width, float thickness, float mass, const Vector4& _color)
	: IndustrialPiece(CreateShape(physx::PxBoxGeometry(lenght, thickness, width)), position, mass, _color), 
	_lenght(lenght)
{
	_surface_area = lenght * width;
}

IndustrialPiece::ForceTransmisionPack PropellerBladePiece::applyPieceReactionForces(const ForceTransmisionPack& force_pack, AttachmentPoint* force_emitter_point)
{	
	Vector3D radial_position = SpaceGeometry::distanceLineToPoint(force_pack.torque_axis_point, force_pack.torque, _transform.p);

	Vector3D flow_velocity = _linear_velocity + _angular_velocity.cross(radial_position);

	Vector3D blade_velocity = _angular_velocity.cross(radial_position);

	// El angulo del ala respecto al movimiento
	float blade_angle = SpaceGeometry::angleBetween(_transform.q.rotate(Vector3D(0, 1, 0).to_vec3()), blade_velocity);
	float attack_angle = blade_angle - atan(_linear_velocity.magnitude() / blade_velocity.magnitude());

	// Cálculo de los coeficientes
	Vector3D blade_normal_in_world = _transform.q.rotate(_surface_normal.to_vec3());

	float cos_velocity_surface = SpaceGeometry::cosineBetween(flow_velocity, blade_normal_in_world);

	float DRAG_COEF = _base_drag_coef + (_max_drag_coef - _base_drag_coef) * cos_velocity_surface;
	float LIFT_COEF = 6 * attack_angle;

	// Fuerzas hidrodinámicas
	Vector3D drag_force = -blade_velocity.normalized() * 0.5 * WATER_DENSITY * pow(flow_velocity.magnitude(), 2)
		* _surface_area * DRAG_COEF;

	Vector3D lift_vec_dir = flow_velocity.cross(radial_position).normalized();

	Vector3D lift_force = lift_vec_dir * 0.5 * WATER_DENSITY * pow(flow_velocity.magnitude(), 2)
		* _surface_area * LIFT_COEF;

	// Fuerzas de reacción
	Vector3D counter_torque = drag_force.cross(radial_position);
	Vector3D thrust_force = SpaceGeometry::projectionVector(lift_force, _linear_velocity);

	return { thrust_force, counter_torque, Vector3D(_transform.p), force_pack.torque_axis_point };
}
