#include "particleSystem.hpp"
#include "GaussianParticleGenerator.hpp"

ParticleSystem::ParticleSystem() {

}
ParticleSystem::~ParticleSystem() {
	for (auto p : _particles)
		delete p;
	_particles.clear();

}
void ParticleSystem::shootParticle(ParticleType t) {
	_particles.push_back(new Particle(t));
}
void ParticleSystem::updateParticles(double t) {
	auto par = _particles.begin();
	while (par != _particles.end()) {
		if ((*par)->getAlive()) {
			(*par)->integrate(t);
			++par;
		}
		else {
			//auto copy = *par;
			delete *par;
			*par = nullptr;
			par = _particles.erase(par);
		}
		/*if (p->getPosition().y < 0.0f) {

		}*/
	}

}
void ParticleSystem::addGenerator(GeneratorType type) {
	ParticleGenerator* generator;
	switch (type)
	{
	case GAUSSIAN:
		_particle_generators.push_back(new GaussianParticleGenerator("GAUSSIAN", 100, { 0,10,0 }, { 0,0,0 }, { 2,1,2 }, { 2,2,2 }, 5.0));
		break;
	default:
		break;
	}
}
void ParticleSystem::shootFirework(int type)
{
}
void ParticleSystem::generateFireworkSystem()
{
	//std::shared_ptr<ParticleGenerator>g(new GaussianParticleGenerator("FireworkGaussian",50,{0,5,0},{0,0,0},{5,5,5},{}))
}
void ParticleSystem::update(double t) {
	for (auto pg : _particle_generators) {
		auto parts = pg->generateParticles();
		for (auto p_ : parts)_particles.push_back(p_);
	}
	updateParticles(t);
}
void ParticleSystem::createFireworkRules() {
	_firework_rules = std::vector<FireworkRule>();

	_firework_rules[0].set(0, 0, 2, { -1, -2, -1 }, { 1, 5, 1 }, 0.999, {});
	_firework_rules[1].set(1, 1, 3, { -3, 0, -3 }, { 3, 0, 3 }, 0.999, { {0, 5} });
	_firework_rules[2].set(2, 2, 4, { -7, 0, -10 }, { 7, 5, 10 }, 0.999, { {1, 5}, {0, 3} });
	_firework_rules[3].set(3, 3, 4, { -5, -2, -5 }, { 5, 3, 5 }, 0.999, { {2, 5}, {1,3}, {0,2} });
	_firework_rules[4].set(4, 0, 2, { -5, 3, 0 }, {5, 5, 0 }, 0.999, { {2, 5}, {1,3}, {0,2}, {0, 3} });
	_firework_rules[5].set(5, 0, 1, { -10, 1, 0 }, { 10, 4, 0 }, 0.999, { {0, 5}, {1,3}, {2,2}, {3, 1}, {4, 5} });
	_firework_rules[6].set(6, 0.5, 1.5, { -3, -0.5, 0 }, { 30.0, 2.5, 0.01 }, 0.999, { {0, 50} });
	_firework_rules[7].set(7, 4, 6, { -5, -0.5, 0 }, { 5, 0, 0 }, 0.999, { {6, 25} });
}
void ParticleSystem::generateFireworks(unsigned type, const Vector3& pos) {
	//if (type >= _firework_rules.size()) return;
	//Particle* new_p = new Firework(pos, { 5.0f,30.0f,.0f }, { 0,-10,0 }*0.99f, _firework_rules[type],
	//	0.99f, 3.0f, _curr_t, type);
	//new_p->setMass(1.0f);

	//_particles.push_back(new_p);
}