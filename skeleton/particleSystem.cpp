#include "particleSystem.hpp"

paticleSystem::paticleSystem() {

}
paticleSystem::~paticleSystem() {
	for (auto p : particleContainer)
		delete p;
	particleContainer.clear();

}
void paticleSystem::shootParticle(ParticleType t) {
	particleContainer.push_back(new Particle(t));
}
void paticleSystem::updateParticles(double t) {
	for (auto p : particleContainer) {
		p->integrate(t);
		/*if (p->getPosition().y < 0.0f) {

		}*/
	}

}
