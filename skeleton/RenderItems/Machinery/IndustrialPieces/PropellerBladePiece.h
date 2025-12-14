#pragma once
#include "IndustrialPiece.h"

class PropellerBladePiece : public IndustrialPiece
{
public:

	/*
		Según qué tipo de movimiento calcula las fuerzas de reacción la pala. REACTION_MODE::LINEAL supone que 
		la pala sigue un movimiento lineal a la hora de calcular las fuerzas de reacción, esto es utilizado en
		el objeto timón del submarino. REACTION_MODE::ANGULAR supone que la pala sigue un movimiento angular,
		pensado para el objeto SubmarineProppeller (las hélices)
	*/
	enum REACTION_MODE { LINEAR, ANGULAR };

	REACTION_MODE reaction_mode;

	PropellerBladePiece(Vector3D position, float lenght = 4, float width = 2, float thickness = 1, float mass = 1, const Vector4& _color = Vector4(1, 1, 1, 1));

	ForceTransmisionPack applyPieceReactionForces(const ForceTransmisionPack& force_pack) override;

private:

	// Se usa la velocidad para calcular la reacción, no se espera recibir ninguna fuerza
	ForceTransmisionPack applyLinearReactionForce(); 
	ForceTransmisionPack applyAngularReactionForce(); 

protected:

	Vector3D _local_wingspan_vector; // Vector envergadora de la pala

	float _surface_area = 0;
	float _lenght;

	float _base_drag_coef = 0.1;
	float _max_drag_coef = 0.3;

	const float WATER_DENSITY = 997;
};