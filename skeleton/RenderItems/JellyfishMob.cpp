#include "JellyfishMob.h"
#include <PxActor.h>
#include <PxRigidDynamic.h>

JellyfishMob::JellyfishMob(Vector3D position, physx::PxPhysics* gPhysics, physx::PxScene* gScene, const Vector4& _color)
	: RenderBody(),
	RenderItem(CreateShape(physx::PxBoxGeometry(1, 1, 1)), gPhysics->createRigidDynamic(physx::PxTransform(position.to_vec3())), _color)
	,_gPhysics(gPhysics), _gScene(gScene)
{
	_body = static_cast<physx::PxRigidDynamic*>(actor); // Obteniendo el cuerpo del actor pasado al render item

	_body->setLinearVelocity({0, 0, 0});
	_body->setAngularVelocity({ 0, 0, 0 });

	shape = CreateShape(physx::PxBoxGeometry(DEFAULT_SIZE.to_vec3())); // Definiendo la forma
	actor->attachShape(*shape);

	// Importante hacerlo despúés de redefinir la forma
	physx::PxRigidBodyExt::updateMassAndInertia(*_body, 1025); // 1025 es la densidad media de una medusa

	_gScene->addActor(*_body); // Añadiendo el cuerpo a la escena para que actúe físicamente

	u_distribution = std::uniform_real_distribution<double>(-1, 1); // Para la dirección aleatoria
}

JellyfishMob::~JellyfishMob()
{
	DeregisterRenderItem(this);
}

void JellyfishMob::update(float t)
{
	addFriction();

	// Hacemos la cuenta atras hasta el próximo impulso
	if (_impulse_count_down > 0)
		_impulse_count_down -= t;

	// Cuando llega la hora de impulsarse la medusa gira
	if (_impulse_count_down <= 0)
	{		
		// Si no se ha empezado a girar se aplica la fueza de giro
		if (!is_rotating)
		{
			Vector3D random_dir;
			random_dir.x = u_distribution(mt);
			random_dir.y = u_distribution(mt);
			random_dir.y = u_distribution(mt);

			_last_applied_torque = random_dir.normalized() * _rotation_force;

			_body->addTorque(_last_applied_torque.to_vec3()); // Empieza a girar
			
			is_rotating = true;
		}

		// Hacemos la cuenta atras hasta que acabe el giro
		if (_rotation_count_down > 0)
			_rotation_count_down -= t;

		if (_rotation_count_down <= 0)
		{
			Vector3 ang_moment = Vector3( // Momento angular
				_body->getAngularVelocity().x * _body->getMassSpaceInertiaTensor().x,
				_body->getAngularVelocity().y * _body->getMassSpaceInertiaTensor().y,
				_body->getAngularVelocity().z * _body->getMassSpaceInertiaTensor().z
			);

			if (_body->getAngularVelocity().magnitude() > 0)
				_body->addTorque(ang_moment * -1); // Rozamiento en el giro

			float intensity = abs(u_distribution(mt)); // Entre 0 y 1
			if (intensity < 0.5) intensity += 0.5;

			Vector3D foward_dir = _body->getGlobalPose().rotate(_base_orientation.to_vec3());
			intensity = 1;
			_body->addForce(foward_dir.normalized().to_vec3() * _impulse_force * intensity);

			// Resetear los timers
			_impulse_count_down = _impulse_period;
			_rotation_count_down = _rotation_duration;
		}
	}
}

void JellyfishMob::addFriction()
{
	// Si la medusa recibe un impacto que la haga rotar demasiado se frenará un poco

	Vector3 ang_moment = Vector3( // Momento angular
		_body->getAngularVelocity().x * _body->getMassSpaceInertiaTensor().x,
		_body->getAngularVelocity().y * _body->getMassSpaceInertiaTensor().y,
		_body->getAngularVelocity().z * _body->getMassSpaceInertiaTensor().z
	);

	if (_body->getAngularVelocity().magnitude() > MAX_ANGULAR_SPEED)
		_body->addTorque(ang_moment * -0.8); // Rozamiento en el giro
}
