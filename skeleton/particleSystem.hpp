#include "particle.hpp"
#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include "particleGenerator.hpp"
#include "firework.hpp"
#include "fireworkRule.hpp"
#include "UniformParticleGenerator.hpp"

enum GeneratorType {
	GAUSSIAN,UNIFORM
};
class ParticleSystem {
protected:
	Vector3 vel;
	RenderItem* renderItem;
	physx::PxTransform pose;//A render item le pasamos dir de este pose,para que se actualice
	/*ParticleType type;*/
	Particle* particle;
	double damping;
	std::list<Particle*>_particles;
	std::list<ParticleGenerator*>_generators;
	std::list<std::shared_ptr<ParticleGenerator>>_particle_generators;
	std::vector<Firework*>_fireworks_pool;
	ParticleGenerator* _firework_gen;
	std::vector<FireworkRule> _firework_rules;
	UniformParticleGenerator* chorro;
public:
	ParticleSystem();
	~ParticleSystem();

	void shootParticle(ParticleType t);
	void updateParticles(double t);
	void generateFireworkSystem();
	void update(double t);
	void addGenerator(GeneratorType type);
	inline void addGenerator(std::shared_ptr<ParticleGenerator> g) { _particle_generators.push_back(g); }
	
	ParticleGenerator* getParticleGenerator(std::string name);

	void activeParticleGenerator(GeneratorType gt);
	//firework
	void shootFirework(int type);
	void onParticleDeath(Particle* p);
	void createFireworkRules();
	void generateFirework(unsigned type, const Vector3& pos);

};
#endif