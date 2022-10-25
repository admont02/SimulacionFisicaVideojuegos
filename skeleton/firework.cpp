#include "firework.hpp"

Firework::Firework(std::list<std::shared_ptr<ParticleGenerator>> partGen, Vector3 pos, Vector3 dir, int life, float r): Particle()
{
	_gens = partGen;
}

Particle* Firework::clone() const
{
	auto newF = new Firework(_gens, pos, vel, 0, 0.f);
	newF->setLifeTime(_remaining_time);
	return newF;
}

std::list<Particle*> Firework::explode()
{
	std::list<Particle*>fireworks;
	for (auto f : _gens) {
		
	}
	return ;
}
