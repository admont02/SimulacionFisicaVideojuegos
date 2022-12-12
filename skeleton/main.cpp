#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "particle.hpp"
#include "particleSystem.hpp"
#include "WorldManager.h"



using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

Particle* par = NULL;
ParticleSystem* shootSys = NULL;
WorldManager* _world;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	shootSys = new ParticleSystem();
	shootSys->generateFireworkSystem();
	//par = new Particle(ParticleType::FIREBALL);
	//solidos rigidos
	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
	PxShape* shapeSuelo = CreateShape(PxBoxGeometry(100, 0.1, 100));
	suelo->attachShape(*shapeSuelo);
	auto item = new RenderItem(shapeSuelo, suelo, { 0,0,0,1 });
	gScene->addActor(*suelo);

	PxRigidStatic* muro = gPhysics->createRigidStatic(PxTransform({ 10,10,-30 }));
	//PxRigidDynamic* wall = gPhysics->createRigidDynamic(PxTransform({ 10,30,-30}));
	PxShape* shapeMuro = CreateShape(PxBoxGeometry(40, 20, 5));
	muro->attachShape(*shapeMuro);
	item = new RenderItem(shapeMuro, muro, {0,1,1,1 });
	gScene->addActor(*muro);
	_world = new WorldManager(gPhysics, gScene);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	//par->integrate(t);
	shootSys->update(t);
	_world->integrate(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();

	delete par;
	par = nullptr;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case 'X': shootSys->shootParticle(FIREBALL);
		break;
	case 'G':
	{
		shootSys->activeParticleGenerator(UNIFORM);
		//shootSys->addGenerator(GAUSSIAN);
		break;
	}
	case 'C':
	{
		shootSys->activeParticleGenerator(GAUSSIAN);
		//shootSys->addGenerator(GAUSSIAN);
		break;
	}

	case 'F':
	{
		shootSys->shootFirework(0);
		break;
	}
	case '1': {
		shootSys->setGravityEffect();
		break;
	}
	case '2': {
		shootSys->turnOnForce("WIND");
		break;
	}
	case '3': {
		shootSys->turnOnForce("WHIRLWIND");
		break;
	}
	case '4': {
		shootSys->explosion();
		break;
	}
	case '5': shootSys->turnOffAllForces();
		break;
	case '+': shootSys->increaseKSpring();
		break;
	case '-': shootSys->decreaseKSpring();
		break;
	case '7': shootSys->generateSpringDemo();
		break;
	case '8': shootSys->generateSlinkyDemo();
		break;
	case '9': shootSys->generateBuoyancyDemo();
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}