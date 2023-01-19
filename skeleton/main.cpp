#include <ctype.h>

#include <PxPhysicsAPI.h>
#include "PxRigidDynamic.h"
#include "PxRigidActor.h"

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include "Player.h"
#include "ParticleSystem.h"

std::string display_text;
int puntos = 0;
using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;

bool canJump = false;
PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;
RenderItem* rIBullets = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;
AuxiliarPlayer* aux = NULL;
PxRigidDynamic* realPlayer;
ParticleSystem* particleSys = NULL;
void createPlayer() {

	realPlayer = gPhysics->createRigidDynamic(PxTransform({ 0,0,0 }));
	realPlayer->setLinearVelocity({ 0,0,0 });
	realPlayer->setAngularVelocity({ 0,0,0 });
	realPlayer->setName("MyPlayer");

	auto shape = CreateShape(PxSphereGeometry(1.0)); realPlayer->attachShape(*shape);
	PxRigidBodyExt::setMassAndUpdateInertia(*realPlayer, 1.0);

	rIBullets = new RenderItem(shape, realPlayer, { 0.5, 0.8, 0.7, 1.0 });
	gScene->addActor(*realPlayer);

	realPlayer->setLinearDamping(0.99);
	realPlayer->setAngularDamping(0.);

}
void texto() {
	display_text = "Puntos:  " + std::to_string(puntos);
	drawText(display_text, 400, 400);
}
void shoot() {
	/*renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(bullet_radius)), &pose, bullet_color);

	newPos = GetCamera()->getEye();
	setPosition(newPos);

	newVel = GetCamera()->getDir() * bullet_velMagnitude;
	setVel(newVel);


	setAcc(bullet_acceleration);
	setDamping(bullet_damping);*/

	auto b = gPhysics->createRigidDynamic(PxTransform({ aux->getPosition() }));
	b->setLinearVelocity({ GetCamera()->getDir() * 200 });
	b->setAngularVelocity({ 0,0,0 });
	b->setName("Bullet");

	auto shape = CreateShape(PxSphereGeometry(1.0)); b->attachShape(*shape);
	PxRigidBodyExt::setMassAndUpdateInertia(*b, .5);

	auto x = new RenderItem(shape, b, { 0.5, 0.8, 0.7, 1.0 });
	gScene->addActor(*b);

	b->setLinearDamping(0.99);
	b->setAngularDamping(0.5);
}
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


	aux = new AuxiliarPlayer();
	createPlayer();
	particleSys = new ParticleSystem(realPlayer, aux, gScene, gPhysics);


	

	particleSys->createSalas();
	particleSys->generateSlinky();
	particleSys->createEnemies({ 300,0,0 });
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds

void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	particleSys->update(t);
	particleSys->moveGameCamera();
	aux->update(t);
	texto();
	//GetCamera()
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
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'W':
		particleSys->movePlayer();
		break;
	case 'C':
		particleSys->activeParticleGenerator(GAUSSIAN);

		break;

	case 'S':particleSys->playerJoinsSlinky(aux, realPlayer);
		break;
	
	case 'X': //shoot();
		particleSys->shootBullet({ particleSys->getPlayerPos() + Vector3(0,4,0)});
		break;
	
	case 'J': 
		particleSys->jumpPlayer();
		break;
	default:
		break;
	}
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	if (actor1->getName() == "Bullet" && actor2->getName() == "Canasta") {
		particleSys->shootFirework(0);
		puntos += 1;
	}
	if (actor1->getName() == "Bullet" && actor2->getName() == "Enemy") {
		puntos += 1;
	}
	if (actor1->getName() == "BulletEnemy" && actor2->getName() == "MyPlayer") {
		puntos -= 1;
	}
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
AuxiliarPlayer* getPlayer() {
	return aux;
}
/*
void WorldManager::eliminarCuerpo(RigidSolid* cuerpo){
	//DeregisterRenderItem(cuerpo->item);
	rfr->deleteRigidRegistry(static_cast<PxRigidDynamic*>(cuerpo->solidType));
	cuerpo->solidType->release();
	DeregisterRenderItem(cuerpo->item);
	delete cuerpo;
	currentNum--;
}


struct RigidSolid {
	PxRigidActor* solidType;
	RenderItem* item;
	vector<string> forcesNames;
	double maxTimeAlive;
	double timeAlive=-1.0;
};


list<RigidSolid*> list_static;


if (PxGeometryQuery::overlap(pelota->item->shape->getGeometry().sphere(), pelota->rigid->getGlobalPose(), porteriaRoja->rI->shape->getGeometry().box(), *porteriaRoja->p))

*/