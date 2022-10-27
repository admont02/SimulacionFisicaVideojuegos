#include "particle.hpp"
#include "particleGenerator.hpp"
#ifndef __UNIFORM_PARTICLE_GENERATOR_H__
#define __UNIFORM_PARTICLE_GENERATOR_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include <random>

class UniformParticleGenerator : public ParticleGenerator
{
protected:
	Vector3 _pos;
	Vector3 _vel;
	std::random_device rand;
	std::default_random_engine e;
	std::uniform_real_distribution<double>unf{ -1,1 };
public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel);

	std::list<Particle*> generateParticles() override;
};
#endif