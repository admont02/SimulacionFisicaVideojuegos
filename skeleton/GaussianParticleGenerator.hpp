#include "particle.hpp"
#include "particleGenerator.hpp"
#ifndef __GAUSSIAN_PARTICLE_GENERATOR_H__
#define __GAUSSIAN_PARTICLE_GENERATOR_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include <random>


class GaussianParticleGenerator : public ParticleGenerator {
private:
	Vector3 std_dev_vel;
	Vector3 std_dev_pos;
	double std_dev_t;
	std::normal_distribution<double>_distribucion_normal{0,1};

public:
	GaussianParticleGenerator();
	~GaussianParticleGenerator();

    std::list<Particle*>generateParticles() override;



};
#endif