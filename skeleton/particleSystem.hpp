#include "particle.hpp"
#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include "particleGenerator.hpp"

enum GeneratorType {
	GAUSSIAN
};
class ParticleSystem {
private:
	Vector3 vel;
	RenderItem* renderItem;
	physx::PxTransform pose;//A render item le pasamos dir de este pose,para que se actualice
	/*ParticleType type;*/
	Particle* particle;
	double damping;
	std::list<Particle*>_particles;
	std::list<ParticleGenerator*>_particle_generators;
public:
	ParticleSystem();
	~ParticleSystem();

	void shootParticle(ParticleType t);
	void updateParticles(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();
	void update(double t);
	void addGenerator(GeneratorType type);


};
#endif