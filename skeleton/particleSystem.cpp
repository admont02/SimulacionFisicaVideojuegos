#include "particleSystem.hpp"

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
	for (auto p : _particles) {
		p->integrate(t);
		/*if (p->getPosition().y < 0.0f) {

		}*/
	}

}
