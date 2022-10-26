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
protected:
	std::string _name;
	Vector3 _mean_pos;
	Vector3 _mean_vel;
	double _generation_probability;
	int _num_particles;
	Particle* _model;
	bool _active = true;
public:
	ParticleGenerator(std::string name, int numPart, Vector3 vel, Vector3 pos);
	~ParticleGenerator() {};

	inline void setParticle(Particle* model) {
		//if (model != nullptr) delete _model;
		_model = model;
	}
	virtual std::list<Particle*>generateParticles() = 0;
	inline std::string getName() { return _name; }
	//void OnParticleDeath(Particle* p);
	//void shootFirework(int type);
	inline void setNumPart(int n) { _num_particles = n; }
	inline void turnOff() { _active = false; }
	inline void turnOn() { _active = true; }

};
#endif