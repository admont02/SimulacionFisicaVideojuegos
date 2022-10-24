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
			auto copy = *par;
			delete copy;
			copy = nullptr;
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
		_particle_generators.push_back(new GaussianParticleGenerator("GAUSSIAN", 100, { 0,10,0 }, { 30,30,30 }, { 2,1,2 }, { 2,2,2 }, 5.0));
		break;
	default:
		break;
	}
}
void ParticleSystem::update(double t) {
	for (auto pg : _particle_generators) {
		auto parts = pg->generateParticles();
		for (auto p_ : parts)_particles.push_back(p_);
	}
	updateParticles(t);
}