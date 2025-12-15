#include "PropellerBladePiece.h"

#include "../../../Utils/SpaceGeometry.h"

PropellerBladePiece::PropellerBladePiece(Vector3D position, float lenght, float width, float thickness, float mass, const Vector4& _color)
	: IndustrialPiece(CreateShape(physx::PxBoxGeometry(lenght, thickness, width)), position, mass, _color), 
	_lenght(lenght)
{
	reaction_mode = REACTION_MODE::LINEAR;

	_surface_area = lenght * width;
	_surface_normal = Vector3D(0, 1, 0);
	_local_wingspan_vector = Vector3D(1, 0, 0) * lenght; // Definimos la envergadura de la pala
}

IndustrialPiece::ForceTransmisionPack PropellerBladePiece::applyPieceReactionForces(const ForceTransmisionPack& force_pack)
{
	if (reaction_mode == REACTION_MODE::LINEAR)
		return applyLinearReactionForce();

	// if (reaction_mode == REACTION_MODE::ANGULAR)
	return applyAngularReactionForce();
}

IndustrialPiece::ForceTransmisionPack PropellerBladePiece::applyLinearReactionForce()
{
	Vector3D flow_velocity = _linear_velocity; // CAMBIÓ

	if (flow_velocity.magnitude() <= 0)
		return { Vector3D(), Vector3D(), Vector3D(), Vector3D() };

	// El ángulo de ataque es el mismo que el angulo de la pala respecto al movimiento

	Vector3D blade_normal_in_world = _transform.q.rotate(_surface_normal.to_vec3()); // Normal de la pala

	float attack_angle = SpaceGeometry::angleBetween(blade_normal_in_world, flow_velocity) - physx::PxPi * 0.5; // CAMBIÓ
	
	// Cálculo de los coeficientes

	float cos_velocity_surface = SpaceGeometry::cosineBetween(flow_velocity, blade_normal_in_world);

	float DRAG_COEF = _base_drag_coef + (_max_drag_coef - _base_drag_coef) * cos_velocity_surface;
	float LIFT_COEF = 6 * attack_angle;

	// Fuerzas hidrodinámicas

	// Arrastre
	float v2 = pow(flow_velocity.magnitude(), 2);
	Vector3D v = flow_velocity.normalized();

	Vector3D drag_force = -v * 0.5 * WATER_DENSITY * v2 * _surface_area * DRAG_COEF;

	// Sustentación
	Vector3D wingspan_vector = _transform.q.rotate(_local_wingspan_vector.to_vec3()); // Vector envergadura de la pala
	Vector3D lift_vec_dir = flow_velocity.cross(wingspan_vector).normalized(); // CAMBIÓ

	Vector3D lift_force = lift_vec_dir * 0.5 * WATER_DENSITY * pow(flow_velocity.magnitude(), 2)
		* _surface_area * LIFT_COEF * 10; // TODO: Quitar el 10

	// Fuerzas de reacción
	Vector3D thrust_force = lift_force + drag_force; // CAMBIÓ

	return { thrust_force, Vector3D(), Vector3D(_transform.p), Vector3D() }; // CAMBIÓ
}

IndustrialPiece::ForceTransmisionPack PropellerBladePiece::applyAngularReactionForce()
{
	Vector3D radial_position = SpaceGeometry::distanceLineToPoint(_rotation_point, _angular_velocity, _transform.p);

	Vector3D blade_rotational_velocity = _angular_velocity.cross(radial_position);

	Vector3D flow_velocity = _linear_velocity + blade_rotational_velocity;

	if (flow_velocity.magnitude() <= 0)
		return { Vector3D(), Vector3D(), Vector3D(), Vector3D() };

	// El angulo del ala respecto al movimiento
	float blade_angle = SpaceGeometry::angleBetween(_transform.q.rotate(_surface_normal.to_vec3()), blade_rotational_velocity);
	float attack_angle = blade_angle - SpaceGeometry::angleBetween(blade_rotational_velocity, flow_velocity);

	// Cálculo de los coeficientes
	Vector3D blade_normal_in_world = _transform.q.rotate(_surface_normal.to_vec3()); // Normal de la pala

	float cos_velocity_surface = SpaceGeometry::cosineBetween(flow_velocity, blade_normal_in_world);

	float DRAG_COEF = _base_drag_coef + (_max_drag_coef - _base_drag_coef) * cos_velocity_surface;
	float LIFT_COEF = 6 * attack_angle;

	// Fuerzas hidrodinámicas

	// Arrastre
	float v2 = pow(flow_velocity.magnitude(), 2);
	Vector3D v = flow_velocity.normalized();

	Vector3D drag_force = -v * 0.5 * WATER_DENSITY * v2 * _surface_area * DRAG_COEF;

	// Sustentación
	Vector3D lift_vec_dir = flow_velocity.cross(radial_position).normalized();

	Vector3D lift_force = lift_vec_dir * 0.5 * WATER_DENSITY * pow(flow_velocity.magnitude(), 2)
		* _surface_area * LIFT_COEF;

	// Fuerzas de reacción
	Vector3D counter_torque = drag_force.cross(radial_position);
	Vector3D thrust_force = lift_force;

	return { thrust_force, counter_torque, Vector3D(_transform.p), _rotation_point };
}
