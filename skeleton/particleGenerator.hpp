#include "particle.hpp"
#ifndef __PARTGEN_H__
#define __PARTGEN_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
//enum ParticleType {
//	FIREBALL
//};

class particleGenerator {
public:
	particleGenerator();
	~particleGenerator();

	void shootParticle(ParticleType t);
	void updateParticles(double t);

private:
	Vector3 vel;
	RenderItem* renderItem;
	physx::PxTransform pose;//A render item le pasamos dir de este pose,para que se actualice
	/*ParticleType type;*/
	Particle* particle;
	double damping;
	std::list<Particle*>particleContainer;
};
#endif