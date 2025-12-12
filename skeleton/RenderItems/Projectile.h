#pragma once
#include "../RenderUtils.hpp"
#include "../Utils/Vector3D.h"

class Projectile : public RenderItem
{
public:
	
	// El proyectil recibe la posición de aparición y la fuerza del disparo
	Projectile(
		physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3D position, 
		Vector3D velocity, float mass, float radius, const Vector4& _color = {1, 1, 1, 1});

protected:
	physx::PxPhysics* _gPhysics;
	physx::PxScene* _gScene;
	physx::PxRigidDynamic* _body; // Actor físico
};