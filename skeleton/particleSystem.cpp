#include "particleSystem.hpp"
#include "GaussianParticleGenerator.hpp"

ParticleSystem::ParticleSystem() {
	chorro = new UniformParticleGenerator({ 0,0,0 }, { 0,0,0 });
	chorro->setParticle(new Particle(FIREBALL, { 0,0,0 }, { 0,10,-40 }, { 0,-10,0 }, 1.0, 0.99, 2.0, { 1,0.5,0,1 }));
	std::shared_ptr<ParticleGenerator> it = std::shared_ptr<ParticleGenerator>(chorro);
	_particle_generators.push_back(it);
	//_generators.push_back(chorro);
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
	case GAUSSIAN:
		break;
	case UNIFORM: chorro->turnOn();
		break;
	default:
		break;
	}
}
void ParticleSystem::shootFirework(int type)
{
	//generateFireworkSystem();


	auto f = _fireworks_pool[type]->clone();
	/*_fireworks_pool[type]->setPosition(Vector3(347384738974, 0, 0));*/
	f->setPosition({ 0,0,0 });
	//	auto g = getParticleGenerator("FIREWORK_GAUSSIAN");
		//g->setParticle(f);

		//x->setPosition({ 0,0,0 });

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
	//std::shared_ptr<ParticleGenerator>g(new GaussianParticleGenerator("FireworkGaussian", 50, { 0,5,0 }, { 0,0,0 }, { 5,5,5 }, {}));
	std::shared_ptr<ParticleGenerator>g(new GaussianParticleGenerator("FIREWORK_GAUSSIAN", FIREWORK, 100, { 0,10,0 }, { 0,0,0 }, { 2,1,2 }, { 2,2,2 }, 5.0));
	//	_particle_generators.push_back(g);
	auto x = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 20, 0), Vector3(0, -10, 0), 1.0, 0.99);
	g->setNumPart(0);
	x->addGenerator(g);
	//	x->addGenerator(_particle_generators.front()); //x->addGenerator(rocket);
	x->setLifeTime(3.0);
	_fireworks_pool.push_back(x);
	_particle_generators.push_back(g);
	g->turnOff();

	/*x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 30, 0), Vector3(0, 2, 0), 0.9999, 1.0, Firework::LINEAR);
	x->addGenerator(particleGen_list.front());
	x->setTimeAlive(8.0);
	fireworks_pool.push_back(x);

	auto rocket = particleGen_list.begin(); rocket++;
	x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 30, 0), Vector3(0, 2, 0), 0.9999, 1.0, Firework::CIRCULAR);
	x->addGenerator(*rocket);
	x->setTimeAlive(8.0);
	fireworks_pool.push_back(x);*/
}
void ParticleSystem::update(double t) {
	for (auto pg : _particle_generators) {
		if (pg->isOn()) {
			auto parts = pg->generateParticles();
			for (auto p_ : parts)_particles.push_back(p_);
		}


	}

	/*if (chorro != nullptr && chorro->isOn()) {

		std::list<Particle*> newParticles = chorro->generateParticles();
		for (auto a : newParticles)
			_particles.push_back(a);
		newParticles.clear();
	}*/
	updateParticles(t);
}
void ParticleSystem::createFireworkRules() {
	_firework_rules = std::vector<FireworkRule>();

	_firework_rules[0].set(0, 0, 2, { -1, -2, -1 }, { 1, 5, 1 }, 0.999, {});
	_firework_rules[1].set(1, 1, 3, { -3, 0, -3 }, { 3, 0, 3 }, 0.999, { {0, 5} });
	_firework_rules[2].set(2, 2, 4, { -7, 0, -10 }, { 7, 5, 10 }, 0.999, { {1, 5}, {0, 3} });
	_firework_rules[3].set(3, 3, 4, { -5, -2, -5 }, { 5, 3, 5 }, 0.999, { {2, 5}, {1,3}, {0,2} });
	_firework_rules[4].set(4, 0, 2, { -5, 3, 0 }, { 5, 5, 0 }, 0.999, { {2, 5}, {1,3}, {0,2}, {0, 3} });
	_firework_rules[5].set(5, 0, 1, { -10, 1, 0 }, { 10, 4, 0 }, 0.999, { {0, 5}, {1,3}, {2,2}, {3, 1}, {4, 5} });
	_firework_rules[6].set(6, 0.5, 1.5, { -3, -0.5, 0 }, { 30.0, 2.5, 0.01 }, 0.999, { {0, 50} });
	_firework_rules[7].set(7, 4, 6, { -5, -0.5, 0 }, { 5, 0, 0 }, 0.999, { {6, 25} });
}
void ParticleSystem::generateFirework(unsigned type, const Vector3& pos) {
	//if (type >= _firework_rules.size()) return;
	//Particle* new_p = new Firework(pos, { 5.0f,30.0f,.0f }, { 0,-10,0 }*0.99f, _firework_rules[type],
	//	0.99f, 3.0f, _curr_t, type);
	//new_p->setMass(1.0f);

	//_particles.push_back(new_p);
}