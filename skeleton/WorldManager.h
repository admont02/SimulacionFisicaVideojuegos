#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include "RenderUtils.hpp"
#include <list>
using namespace physx;

class WorldManager
{
protected:

	std::list<PxRigidDynamic*> _solids;
	RenderItem* item;
	PxPhysics* _physics;
	PxScene* _scene;
	RenderItem* rI;
	Vector3 size = { 5, 5, 5 };
	int maxSolids;

public:
	WorldManager(PxPhysics* gPhysics, PxScene* gScene) {
		_physics = gPhysics;
		_scene = gScene;
		maxSolids = 200;
	}
	~WorldManager();


	inline void generateSolid() {
		PxRigidDynamic* solid= _physics->createRigidDynamic(PxTransform({ 0, 40, 40 }));
		solid->setLinearVelocity({0,10,10});
		solid->setAngularVelocity({ 0, 0, 0 });

		PxShape* shape = CreateShape(PxBoxGeometry(size));
		solid->attachShape(*shape);

		solid->setMassSpaceInertiaTensor({ size.y * size.z , size.x * size.z, size.x * size.y });
		rI = new RenderItem(shape, solid, { 0.4, 0.9, 1, 1 });
		_scene->addActor(*solid);

		_solids.push_back(solid);
	}
	//void addStaticObject();
	inline void integrate(double t) {
		if (_solids.size() >= maxSolids) return; 
		generateSolid();
	}
	//void addForce();





};