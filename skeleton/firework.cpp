#include "firework.hpp"

Firework::Firework(std::list<std::shared_ptr<ParticleGenerator>> g, Vector3 p, Vector3 v, Vector3 a, float mass,float damping): Particle(FIREWORK,p,v,a,mass,damping)
{
	_gens = g;
}

Particle* Firework::clone() const
{
	auto newF = new Firework(_gens,pos, vel,a, mass, damping);
	newF->setLifeTime(_remaining_time);
	return newF;
}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*>fireworks;
	for (auto f : _gens) {
		f->setPosition(pos);
		f->setVelocity(vel);

		auto part = f->generateParticles();
		for (auto fw : part)
			fireworks.push_back(fw);
	}
	return fireworks;
}
