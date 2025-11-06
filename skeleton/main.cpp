#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "RenderItems/Axis3D.h"
#include "RenderItems/Particle.h"

#include "ParticleSystem/ParticleSystem.h"
#include "ParticleSystem/ParticleGenerators/UniformParticleGenerator.h"
#include "ParticleSystem/ParticleGenerators/RoundAreaRangeGenerator.h"

#include "ParticleSystem/ForceGenerators/GravityForceGenerator.h"
#include "ParticleSystem/ForceGenerators/WindForceGenerator.h"
#include "ParticleSystem/ForceGenerators/TornadoForceGenerator.h"

#include "RenderItems/Machinery/IndustrialPieces/IndustrialPiece.h"
#include "RenderItems/Machinery/Submarine.h"

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

Axis3D* axis;
std::vector<Particle*> projectiles;
float projectileSpeed = 5;// 30;

ParticleSystem* pSys;
RoundAreaRangeGenerator* rain_generator;

IndustrialPiece* piece1;
IndustrialPiece* piece2;
IndustrialPiece::AttachmentPoint* ap1;
IndustrialPiece::AttachmentPoint* ap2;

RenderItem* Water;
Submarine* submarine;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//ball = new RenderItem(CreateShape(PxSphereGeometry(0.5)), new PxTransform(PxVec3(0, 0, 0)), Vector4(1, 0, 0, 1));
	//RegisterRenderItem(ball);
	axis = new Axis3D();

	pSys = new ParticleSystem();
	pSys->referenceParticleGenerator(
		std::make_shared<RoundAreaRangeGenerator>(
			pSys, new Particle(Vector3D(0, 20, 0), Vector3D(0, 0, 0)), 0.1, 10, 
			RoundAreaRangeGenerator::CIRCLE, 50
		)
	);
	pSys->referenceForceGenerator(std::make_shared<GravityForceGenerator>(pSys, 25));
	pSys->referenceForceGenerator(std::make_shared<WindForceGenerator>(pSys, Vector3D(20, 0, 0), 0.2));
	//pSys->referenceParticleGenerator(std::make_shared<UniformParticleGenerator>(pSys, new Particle(Vector3D(0, 0, 0), Vector3D(1, 5, 1)), 1, 2));
	//pSys->referenceForceGenerator(std::make_shared<GravityForceGenerator>(pSys, -25));
	//pSys->referenceForceGenerator(std::make_shared<WindForceGenerator>(pSys, Vector3D(5, 0, 0), 0.2));
	//pSys->referenceForceGenerator(std::make_shared<TornadoForceGenerator>(pSys, Vector3D(), Vector3D(), 0.2, 0, 2));
	
	physx::PxTransform transform = physx::PxTransform(Vector3D().to_vec3());
	Water = new RenderItem(CreateShape(physx::PxBoxGeometry(100, 100, 100)), new PxTransform(PxVec3(0, 0, 0)), Vector4(0, 0, 1, 0.1));
	
	submarine = new Submarine();
	pSys->referenceParticleGenerator(std::make_shared<UniformParticleGenerator>(pSys, submarine->motor_bubble(), 0.2, 3));

	// Pruebas con IndustriualPiece
	
	/*
	piece1 = new IndustrialPiece(Vector3D(4, 0, 4), 10, Vector4(1, 0, 1, 1));
	ap1 = new IndustrialPiece::AttachmentPoint{ piece1, nullptr, Vector3D(0, 0, 1) };
	piece1->addAttachmentPoint(ap1);

	piece2 = new IndustrialPiece(Vector3D(4, 0, 8), 10, Vector4(0, 1, 1, 1));
	ap2 = new IndustrialPiece::AttachmentPoint{ piece2, nullptr, Vector3D(0, 0, -1) };
	piece2->addAttachmentPoint(ap2);

	ap1->linkTo(ap2);*/
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	for (Particle* projectile : projectiles)
		projectile->integrate(t);

	pSys->update(t);
	
	submarine->update(t);
	/*
		INDUSTRIAL PIECE
	IndustrialPiece::ForceTransmisionPack force_pack{Vector3D(10, 0, 0), Vector3D(0, 0, 0), Vector3D(0,0,0), Vector3D(0,0,0)};
	IndustrialPiece::ForceTransmisionPack force_reaction = piece1->propagateForces(force_pack, ap1);

	Vector3D force_result = force_pack.force + force_reaction.force;
	Vector3D angular_vel = (Vector3D(1, 0, 0) * 2 * t).to_vec3();

	piece1->propagateMotionEffect({ Vector3D(piece1->_transform.p) * 0, force_result * t * 0, angular_vel });*/
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	//DeregisterRenderItem(ball);
	delete axis;
	for (Particle* projectile : projectiles)
		delete projectile;

	delete pSys;

	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	submarine->keyPress(key);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	/*case 'P':
		//projectiles.push_back(new Particle(GetCamera()->getEye(), GetCamera()->getDir() * projectileSpeed, -9.8));
		break;*/
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}