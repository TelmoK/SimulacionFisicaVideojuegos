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
#include "ParticleSystem/ForceGenerators/ThrustForceGenerator.h"

#include "RenderItems/Machinery/IndustrialPieces/IndustrialPiece.h"
#include "RenderItems/Machinery/Submarine.h"
#include "RenderItems/Machinery/SumarinePropeller.h"

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

// -----------------------------------------------------------------------
// DECLARACIONES PROPIAS
// -----------------------------------------------------------------------

Axis3D* axis;

std::unique_ptr<ParticleSystem> snow_particle_sys;
std::unique_ptr<ParticleSystem> general_particle_sys;

std::unique_ptr<physx::PxBoxGeometry> sea_geometry;
std::unique_ptr<physx::PxTransform> sea_transform;
std::unique_ptr<RenderItem> sea_water_block;

// Generadores declarados globalmente para uso de teclas
std::shared_ptr<GravityForceGenerator> snow_gravity_generator;
std::shared_ptr<WindForceGenerator> snow_wind_generator;
std::shared_ptr<GravityForceGenerator> general_gravity_generator;
std::shared_ptr<ThrustForceGenerator> general_thrust_generator;

Submarine* submarine;

constexpr float WATER_DENSITY = 997;

/*IndustrialPiece* piece1;
IndustrialPiece* piece2;
IndustrialPiece::AttachmentPoint* ap1;
IndustrialPiece::AttachmentPoint* ap2;*/
SubmarinePropeller* propeller;


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

	// -------------------------------------------------------------------------------------
	// MONTAJE DE LA ESCENA
	// -------------------------------------------------------------------------------------

	axis = new Axis3D();

	general_particle_sys = std::make_unique<ParticleSystem>();
	snow_particle_sys = std::make_unique<ParticleSystem>();

	// La nieve
	snow_particle_sys->referenceParticleGenerator(
		std::make_shared<RoundAreaRangeGenerator>(
			snow_particle_sys.get(), new Particle(Vector3D(-200, 160, -200), Vector3D(0, 0, 0)), 0.1, 100,
			RoundAreaRangeGenerator::CIRCLE, 400
		)
	);

	snow_gravity_generator = std::make_shared<GravityForceGenerator>(snow_particle_sys.get(), -25);
	snow_wind_generator = std::make_shared<WindForceGenerator>(snow_particle_sys.get(), Vector3D(30, 0, 0), 0.2);

	snow_particle_sys->referenceForceGenerator(snow_gravity_generator);
	snow_particle_sys->referenceForceGenerator(snow_wind_generator);
	
	// El "mar"
	sea_geometry = std::make_unique<physx::PxBoxGeometry>(200, 200, 200);
	sea_transform = std::make_unique<physx::PxTransform>(PxVec3(0, -100, 0));
	sea_water_block = std::make_unique<RenderItem>(CreateShape(*sea_geometry), sea_transform.get(), Vector4(0, 0, 1, 0.1));
	
	// El submarino y demás
	submarine = new Submarine(Vector3D(0, 100, 0), general_particle_sys.get());

	general_gravity_generator = std::make_shared<GravityForceGenerator>(snow_particle_sys.get(), -9.8);
	general_thrust_generator = std::make_shared<ThrustForceGenerator>(general_particle_sys.get(), WATER_DENSITY, -9.8, sea_geometry.get(), sea_transform.get());

	general_particle_sys->referenceForceGenerator(general_thrust_generator);
	general_particle_sys->referenceForceGenerator(general_gravity_generator);

	// Pruebas con IndustriualPiece
	
	propeller = new SubmarinePropeller(4);
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

	snow_particle_sys->update(t);
	general_particle_sys->update(t);
	
	submarine->update(t);
	
	/*	INDUSTRIAL PIECE
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
	delete propeller;

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
	case '0':
	{
		snow_gravity_generator->setActive(!snow_gravity_generator->active());
		break;
	}
	case '9':
	{
		snow_wind_generator->setActive(!snow_wind_generator->active());
		break;
	}
	case '8':
	{
		general_gravity_generator->setActive(!general_gravity_generator->active());
		break;
	}
	case '7':
	{
		general_thrust_generator->setActive(!general_thrust_generator->active());
		break;
	}
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