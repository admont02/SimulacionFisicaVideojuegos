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
	Firework(std::list<std::shared_ptr<ParticleGenerator>> g, Vector3 pos, Vector3 vel,Vector3 a, float mass,float damping);
	~Firework(){};
	//virtual void integrate(double t) override;
   Firework* clone() const;
	inline void addGenerator(std::shared_ptr<ParticleGenerator> g) { _gens.push_back(g); }
	void Firework::explode(std::list<Particle*>& l);
	void deleteGenerators();
};
#endif