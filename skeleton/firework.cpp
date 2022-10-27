#include "firework.hpp"

Firework::Firework(Vector3 p, Vector3 v, Vector3 a, float mass, float damping) : Particle(FIREWORK, p, v, a, mass, damping)
{

}

Firework* Firework::clone() const
{
	auto newF = new Firework(pos, vel, a, mass, damping);
	newF->setLifeTime(_remaining_time);
	for (auto g : _gens) newF->addGenerator(g);
	return newF;
}

void Firework::explode(std::list<Particle*>&l)
{
	
	for (auto f : _gens) {
		auto p = clone();
	
		p->deleteGenerators();
		
		p->setLifeTime(_remaining_time / 2);
		
		(*f).setNumPart(10);
		(*f).setParticle(p);
		f->turnOn();
		auto nL=(f)->generateParticles();
		f->turnOff();

		for (auto fw : nL)
			l.push_back(fw);	
		p->setPosition({ 888888888888,2378902839280,0 });
	}
}

void Firework::deleteGenerators()
{
	while (!_gens.empty())
		_gens.pop_front();

}
