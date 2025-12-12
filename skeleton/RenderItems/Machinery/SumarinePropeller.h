#pragma once
#include "IndustrialPieces/IndustrialPiece.h"
#include "IndustrialPieces/PropellerBladePiece.h"

#include <vector>

class SubmarinePropeller {
public:

	SubmarinePropeller(int blade_num, physx::PxPhysics* gPhysics);
	~SubmarinePropeller();

	Vector3D getInvInerceTensorDiagonal();

	void update(double t);

protected:
	// Puntero a Physics de la escena para calcular el tensor de inercia
	physx::PxPhysics* _gPhysics = nullptr;
	
	// Hacia donde apuntan la hélices antes de aplicar cualquier rotación.
	// Importante para aplicar la fuerza del motor en la dirección correcta.
	Vector3D base_orientation = Vector3D(1, 0, 0); 

	// Masas
	const float SHAFT_MASS = 10;
	const float BOSS_MASS = 10;
	const float BLADE_MASS = 10;

	float _density;// = 997;
	float _mass;

	// Referencia, Posicionamiento y Tamaños de las componentes

	int _blade_num;
	float _blade_angle;

	const Vector3D SHAFT_SIZE = Vector3D(6, 2, 2);
	const Vector3D BOSS_SIZE = Vector3D(4, 4, 4);
	const Vector3D BLADE_SIZE = Vector3D(6, 1, 2);

	IndustrialPiece* _shaft; // Eje
	physx::PxTransform _shaft_local_transform;
	IndustrialPiece::AttachmentPoint* _shaft_ap;

	IndustrialPiece* _boss; // Núcleo
	physx::PxTransform _boss_local_transform;

	std::vector<PropellerBladePiece*> _blades; // Palas
	std::vector<physx::PxTransform> _blades_local_transform;

	std::vector<IndustrialPiece::AttachmentPoint*> _attachment_points;
};