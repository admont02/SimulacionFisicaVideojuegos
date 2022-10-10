#include "particle.hpp"
#ifndef __PARTICLE_GENERATOR_H__
#define __PARTICLE_GENERATOR_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include <string>


class ParticleGenerator {
private:
	std::string _name;
	Vector3 _mean_pos;
	Vector3 _mean_vel;
	double _generation_probability;
	int _num_particles;
	Particle* _model;
public:
	ParticleGenerator();
	~ParticleGenerator();

	void setParticle(Particle* model);
	virtual std::list<Particle*>generateParticles()=0;


	
};
#endif