#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include "RenderUtils.hpp"
#include <list>
#include "SolidForceRegistry.h"
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
	SolidForceRegistry* _solid_force_Reg;
	std::shared_ptr<ForceGenerator>_wind;


	std::list<std::shared_ptr<ForceGenerator>> _force_generators;


public:
	WorldManager(PxPhysics* gPhysics, PxScene* gScene) {
		_physics = gPhysics;
		_scene = gScene;
		maxSolids = 200;
		_solid_force_Reg = new SolidForceRegistry();
		_wind = std::shared_ptr<ForceGenerator>(new WindForceGenerator(0.6, 0, { -200,-100,0 }));
		_wind->_name = "WIND";
		//_wind->active = true;
		_force_generators.push_back(_wind);

	}
	~WorldManager();


	inline void generateSolid() {
		PxRigidDynamic* solid = _physics->createRigidDynamic(PxTransform({ 0, 0, 0 }));
		solid->setLinearVelocity({ 0,0,100 });
		solid->setAngularVelocity({ 0, 0, 0 });

		PxShape* shape = CreateShape(PxBoxGeometry(size));
		solid->attachShape(*shape);

		solid->setMassSpaceInertiaTensor({ size.y * size.z , size.x * size.z, size.x * size.y });
		rI = new RenderItem(shape, solid, { 0.4, 0.9, 1, 1 });
		_scene->addActor(*solid);

		_solids.push_back(solid);


		if (_wind->active)
			_solid_force_Reg->addRegistry(getForceGenerator("WIND"), solid);
	}
	
	inline void integrate(double t) {
		_solid_force_Reg->updateForce(t);
	}

	ForceGenerator* getForceGenerator(std::string name) {
		for (auto f : _force_generators)
			if (f->_name == name)
				return f.get();
		return nullptr;
	}
	inline void applyWind() {
		_wind->active = !_wind->active;
	}


};