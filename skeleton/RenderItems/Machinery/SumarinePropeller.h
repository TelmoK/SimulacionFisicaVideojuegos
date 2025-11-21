#pragma once
#include "IndustrialPieces/IndustrialPiece.h"
#include "IndustrialPieces/PropellerBladePiece.h"

#include <vector>

class SubmarinePropeller {
public:

	SubmarinePropeller(int blade_num);
	~SubmarinePropeller();

protected:

	int _blade_num;
	const Vector3D SHAFT_SIZE = Vector3D(6, 2, 2);
	const Vector3D BOSS_SIZE = Vector3D(4, 4, 4);
	const Vector3D BLADE_SIZE = Vector3D(6, 1, 2);

	IndustrialPiece* _shaft; // Eje
	IndustrialPiece* _boss; // Núcleo
	std::vector<PropellerBladePiece*> _blades; // Palas
};