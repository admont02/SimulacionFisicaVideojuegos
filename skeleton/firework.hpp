#ifndef __FIREWORK_H__
#define __FIREWORK_H__
#include "particle.hpp"
#include <memory>
#include <list>
#include "particleGenerator.hpp"

class Firework : public Particle {
protected:
	std::list<std::shared_ptr<ParticleGenerator>> _gens;
public:
	Firework(std::list<std::shared_ptr<ParticleGenerator>> partGen,Vector3 pos, Vector3 dir,int life, float r);
	~Firework(){};
	//virtual void integrate(double t) override;
	virtual Particle* clone() const;

	std::list<Particle* >explode();
};
#endif