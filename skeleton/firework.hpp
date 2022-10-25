#ifndef __FIREWORK_H__
#define __FIREWORK_H__
#include "particle.hpp"
#include <memory>
#include <list>
#include "particleGenerator.hpp"

class Firework : public Particle {
	
	int age; //edad del firework, si es 0 no se generan más
	std::list<std::shared_ptr<ParticleGenerator>> _gens;
public:
	Firework(Vector3 pos, Vector3 dir, std::list<std::shared_ptr<ParticleGenerator>> lG, float radius, int a);
	~Firework() = default;
	//virtual void integrate(double t) override;
	virtual Particle* clone() const;

	inline void addGenerator(std::shared_ptr<ParticleGenerator> pG) { _gens.push_back(pG); };

	std::list<Particle* >explode();
};
#endif