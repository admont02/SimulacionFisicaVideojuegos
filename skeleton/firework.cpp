#include "firework.hpp"

Firework::Firework(Vector3 p, Vector3 v, Vector3 a, float mass,float damping): Particle()
{
}

Firework* Firework::clone() const
{
	auto newF = new Firework(pos, vel,a, mass, damping);
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
