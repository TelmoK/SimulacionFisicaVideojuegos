#include "Projectile.h"

Projectile::Projectile(
	physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3D position,
	Vector3D velocity, float mass, float radius, const Vector4& _color
)
: RenderItem(CreateShape(physx::PxSphereGeometry(radius)), gPhysics->createRigidDynamic(physx::PxTransform(position.to_vec3())), _color)
	, _gPhysics(gPhysics), _gScene(gScene)
{
	_body = static_cast<physx::PxRigidDynamic*>(actor); // Obteniendo el cuerpo del actor pasado al render item

	_body->setLinearVelocity(velocity.to_vec3());
	_body->setAngularVelocity({ 0, 0, 0 });

	float volume = (4 / 3) * physx::PxPi * pow(radius, 3);

	physx::PxRigidBodyExt::updateMassAndInertia(*_body, mass / volume);

	shape = CreateShape(physx::PxSphereGeometry(radius)); // Definiendo la forma
	actor->attachShape(*shape);

	_gScene->addActor(*_body); // Añadiendo el cuerpo a la escena para que actúe físicamente
}

