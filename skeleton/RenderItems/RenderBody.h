#pragma once
#include "DynamicEntity.h"

#include <PxPhysics.h>
#include <PxRigidDynamic.h>

class RenderBody : public DynamicEntity
{
public:
	RenderBody() {}
	~RenderBody() {
		_body->release(); // Borramos el cuerpo de la escena
	}

	void setBody(physx::PxRigidDynamic* body) {
		_body = body;
	}

	// Devuelve el puntero al actor dinámico rígido
	physx::PxRigidDynamic* body() {
		return _body;
	}

protected:

	physx::PxRigidDynamic* _body; // El actor dinámico del objeto
};