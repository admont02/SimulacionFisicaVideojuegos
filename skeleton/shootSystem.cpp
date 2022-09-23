#include "shootSystem.hpp"

shootSystem::shootSystem() {

}
shootSystem::~shootSystem() {
	for (auto p : particleContainer)
		delete p;
	particleContainer.clear();

}
void shootSystem::shootParticle(ParticleType t) {
	particleContainer.push_back(new Particle(t));
}
void shootSystem::updateParticles(double t) {
	for (auto p : particleContainer) {
		p->integrate(t);
		/*if (p->getPosition().y < 0.0f) {

		}*/
	}

}
