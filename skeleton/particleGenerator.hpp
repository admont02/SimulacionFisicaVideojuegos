
#ifndef __PARTICLE_GENERATOR_H__
#define __PARTICLE_GENERATOR_H__
#include "Particle.h"
#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include <string>


class ParticleGenerator {
protected:
	
	Vector3 _mean_pos;
	Vector3 _mean_vel;
	double _generation_probability;
	int _num_particles;
	Particle* _model;
	bool _active;
public:

	~ParticleGenerator() {};

	inline void setParticle(Particle* model) {
		_model = model;
	}
	virtual std::list<Particle*>generateParticles()=0;
	virtual void generateParticles(std::list<Particle*>& l){}
	inline std::string getName() { return _name; }
	
	inline void setNumPart(int n) { _num_particles = n; }
	inline void turnOff() { _active = false; }
	inline void turnOn() { _active =!_active; }
	inline bool isOn() { return _active; }
	std::string _name;

};
#endif