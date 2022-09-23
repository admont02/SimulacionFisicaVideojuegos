#include "particle.hpp"
#ifndef __SHOOTSYS_H__
#define __SHOOTSYS_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
//enum ParticleType {
//	FIREBALL
//};

class shootSystem {
public:
	shootSystem();
	~shootSystem();

	void shootParticle(ParticleType t);
	void updateParticles(double t);

private:
	Vector3 vel;
	RenderItem* renderItem;
	physx::PxTransform pose;//A render item le pasamos dir de este pose,para que se actualice
	/*ParticleType type;*/
	Particle* particle;
	double damping;
	std::vector<Particle*>particleContainer;
};
#endif