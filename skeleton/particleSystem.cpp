#include "particleSystem.hpp"
#include "GaussianParticleGenerator.hpp"

ParticleSystem::ParticleSystem() {
	chorro = new UniformParticleGenerator({ 0,0,0 }, { 0,0,0 });
	chorro->setParticle(new Particle(FIREBALL, { 0,0,0 }, { 0,10,-40 }, { 0,-10,0 }, 2.0, 0.99, 2.0, { 0.2,0.5,0,1 }));
	std::shared_ptr<ParticleGenerator> it = std::shared_ptr<ParticleGenerator>(chorro);
	_particle_generators.push_back(it);
	chorroGauss= new GaussianParticleGenerator("GAUSSIAN", FIREBALL, 100, { 0,10,0 }, { 0,0,0 }, { 2,1,2 }, { 2,2,2 }, 5.0);
	chorroGauss->setParticle(new Particle(FIREBALL, { 0,0,0 }, { 0,10,-40 }, { 0,-10,0 }, 2.0, 0.99, 2.0, { 1,0.7,1,1 }));
	std::shared_ptr<ParticleGenerator> it2 = std::shared_ptr<ParticleGenerator>(chorroGauss);
	_particle_generators.push_back(it2);
}
ParticleSystem::~ParticleSystem() {
	while (!_particles.empty())
		_particles.pop_front();
	while (!_generators.empty())
		_particle_generators.pop_front();
	for (int i = 0; i < _fireworks_pool.size(); i++)
		delete _fireworks_pool[i];
	_fireworks_pool.clear();
}
void ParticleSystem::shootParticle(ParticleType t) {
	_particles.push_back(new Particle(t, pose.p, vel, { 0,10,0 }, 10.0, 0.99));
}
void ParticleSystem::updateParticles(double t) {
	auto par = _particles.begin();
	while (par != _particles.end()) {
		if ((*par)->getAlive()) {
			(*par)->integrate(t);
			++par;
		}
		else {
			onParticleDeath(*par);
			//auto copy = *par;
			delete* par;
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

		_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new GaussianParticleGenerator("GAUSSIAN", FIREBALL, 100, { 0,10,0 }, { 0,0,0 }, { 2,1,2 }, { 2,2,2 }, 5.0)));
		break;
	default:
		break;
	}
}
ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	for (auto p : _particle_generators)
		if (p->getName() == name)
			return p.get();
	return nullptr;
}
void ParticleSystem::activeParticleGenerator(GeneratorType gt) {
	switch (gt)
	{
	case GAUSSIAN: chorroGauss->turnOn();
		break;
	case UNIFORM: chorro->turnOn();
		break;
	default:
		break;
	}
}
void ParticleSystem::shootFirework(int type)
{


	auto f = _fireworks_pool[type]->clone();
	f->setPosition({ 0,0,0 });

	_particles.push_back(f);
}
void ParticleSystem::onParticleDeath(Particle* p)
{
	auto* f = dynamic_cast<Firework*>(p);
	if (f != nullptr)
		f->explode(_particles);

}
void ParticleSystem::generateFireworkSystem()
{
	std::shared_ptr<ParticleGenerator>g(new GaussianParticleGenerator("FIREWORK_GAUSSIAN", FIREWORK, 100, { 0,10,0 }, { 0,0,0 }, { 2,1,2 }, { 2,2,2 }, 5.0));
	auto f = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 25, 0), Vector3(0, -10, 0), 1.0, 0.99);
	g->setNumPart(0);
	f->addGenerator(g);
	f->setLifeTime(3.0);
	_fireworks_pool.push_back(f);
	_particle_generators.push_back(g);
	g->turnOff();
}
void ParticleSystem::update(double t) {
	for (auto pg : _particle_generators) {
		if (pg->isOn()) {
			auto parts = pg->generateParticles();
			for (auto p_ : parts)_particles.push_back(p_);
		}


	}
	updateParticles(t);
}

