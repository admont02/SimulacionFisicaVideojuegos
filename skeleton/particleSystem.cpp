#include "ParticleSystem.h"
//#include "GaussianParticleGenerator.hpp"

ParticleSystem::ParticleSystem(physx::PxRigidDynamic* realPlayer, AuxiliarPlayer* p, physx::PxScene* scene, physx::PxPhysics* physics) {
	myPlayer = realPlayer;
	gScene = scene;
	gPhysics = physics;
	/*chorro = new UniformParticleGenerator({ 0,0,0 }, { 0,0,0 });
	chorro->setParticle(new Particle(FIREBALL, { 0,0,0 }, { 0,10,-40 }, { 0,-10,0 }, 2.0, 0.99, 2.0, { 0.2,0.5,0,1 }));
	std::shared_ptr<ParticleGenerator> it = std::shared_ptr<ParticleGenerator>(chorro);
	_particle_generators.push_back(it);*/
	chorroGauss = new GaussianParticleGenerator("GAUSSIAN", FIREBALL, 1, { 0,5,0 }, { 80,0,0 }, { 2,1,2 }, { 2,2,2 }, 3.0);
	chorroGauss->_name = "CHORRO";
	chorroGauss->setParticle(new Particle(FIREBALL, { 0,0,0 }, { 0,10,-40 }, { 0,-10,0 }, 2.0, 0.99, 2.0, { 1,0.7,1,1 }));
	tornadoCoor = 80;
	std::shared_ptr<ParticleGenerator> it2 = std::shared_ptr<ParticleGenerator>(chorroGauss);
	chorroGauss->turnOn();
	_particle_generators.push_back(it2);
	_circleGen = std::shared_ptr<ParticleGenerator>(new CircleGenerator());
	_particle_generators.push_back(_circleGen);
	generateFireworkSystem();
	aux = p;
	_force_reg = new ParticleForceRegistry();
	_solid_reg = new SolidForceRegistry();
	_grav = std::shared_ptr<ForceGenerator>(new GravityForceGenerator({ 0,-9.8,0 }));
	_grav->_name = "GRAV";
	_force_generators.push_back(_grav);

	_wind = std::shared_ptr<ForceGenerator>(new WindForceGenerator(0.6, 0, { 20,0,0 }));
	_wind->_name = "WIND";
	//_wind->active = true;
	_force_generators.push_back(_wind);

	_whirlwind = std::shared_ptr<ForceGenerator>(new WhirlwindForceGenerator(1.9, { 1,0,0 }, { 0,0,0 }));
	_whirlwind->_name = "WHIRLWIND";
	_whirlwind->active = true;
	_force_generators.push_back(_whirlwind);

	_exp = new ExplosionForceGenerator({ 0,0,0 }, 5000, 100);
	_exp->_name = "EXPLOSION";
	//_exp->active = true;
	//_force_generators.push_back(_exp);
}
ParticleSystem::~ParticleSystem() {
	while (!_particles.empty())
		_particles.pop_front();
	while (!_particle_generators.empty())
		_particle_generators.pop_front();
	while (!_force_generators.empty())
		_force_generators.pop_front();
	//for (int i = 0; i < _fireworks_pool.size(); i++)
	//	delete _fireworks_pool[i];
	//_fireworks_pool.clear();

	delete _exp;
	//borrar balas
	auto s = _bullets.begin();
	while (s != _bullets.end()) {
		releaseSolid(*s);
		s = _bullets.erase(s);
	}
	//borrar enemigos
	auto e = _enemies.begin();
	while (e != _enemies.end()) {
		releaseSolid(*e);
		e = _bullets.erase(e);
	}

	auto t = _statics.begin();
	while (e != _statics.end()) {
		releaseSolid(*t);
		t = _bullets.erase(t);
	}

}
void ParticleSystem::shootParticle(ParticleType t) {
	_particles.push_back(new Particle(t, pose.p, vel, { 0,10,0 }, 10.0, 0.99));
}
void ParticleSystem::updateParticles(double t) {
	auto par = _particles.begin();
	while (par != _particles.end()) {
		if ((*par)->getAlive()) {
			(*par)->integrate(t);
			if (getForceGenerator("WIND")->active)
				_force_reg->addRegistry(getForceGenerator("WIND"), *par);
			//if (getForceGenerator("WHIRLWIND")->active && !(*par)->isOnWind) {
			//	/*_force_reg->addRegistry(getForceGenerator("WHIRLWIND"), *par);
			//	(*par)->isOnWind = true;*/

			//}
			/*if (getForceGenerator("EXPLOSION")->active)
				_force_reg->addRegistry(getForceGenerator("EXPLOSION"), *par);*/
			++par;
		}
		else {
			_force_reg->deleteParticleRegistry(*par);
			onParticleDeath(*par);
			//auto copy = *par;
			delete* par;
			*par = nullptr;
			par = _particles.erase(par);
		}
		/*if (p->getPosition().y < 0.0f) {

		}*/
	}
	auto ts = _tornado.begin();
	while (ts != _tornado.end()) {
		if ((*ts)->getAlive()) {
			(*ts)->integrate(t);
			if (getForceGenerator("WIND")->active)
				_force_reg->addRegistry(getForceGenerator("WIND"), *ts);
			if (getForceGenerator("WHIRLWIND")->active && !(*ts)->isOnWind) {
				_force_reg->addRegistry(getForceGenerator("WHIRLWIND"), *ts);
				(*ts)->isOnWind = true;

			}
			/*if (getForceGenerator("EXPLOSION")->active)
				_force_reg->addRegistry(getForceGenerator("EXPLOSION"), *par);*/
			++ts;
		}
		else {
			_force_reg->deleteParticleRegistry(*ts);
			onParticleDeath(*ts);
			//auto copy = *par;
			delete* ts;
			*ts = nullptr;
			ts = _tornado.erase(ts);
		}
		/*if (p->getPosition().y < 0.0f) {

		}*/
	}

}
void ParticleSystem::addGenerator(GeneratorType type) {
	/*ParticleGenerator* generator;
	switch (type)
	{
	case GAUSSIAN:

		_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new GaussianParticleGenerator("GAUSSIAN", FIREBALL, 100, { 0,10,0 }, { 0,0,0 }, { 2,1,2 }, { 2,2,2 }, 5.0)));
		break;
	default:
		break;
	}*/
}
//ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
//{
//	for (auto p : _particle_generators)
//		if (p->getName() == name)
//			return p.get();
//	return nullptr;
//}
ForceGenerator* ParticleSystem::getForceGenerator(std::string name)
{
	for (auto f : _force_generators)
		if (f->_name == name)
			return f.get();
	return nullptr;
}
void ParticleSystem::activeParticleGenerator(GeneratorType gt) {
	switch (gt)
	{
	case GAUSSIAN: chorroGauss->turnOn();
		break;
		//case UNIFORM: chorro->turnOn();
		//	break;
	default:
		break;
	}
}
void ParticleSystem::shootFirework(int type)
{
	auto f = _fireworks_pool[type]->clone();
	f->setPosition({ -2.0f, 14.0f, 0.0f });

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
	auto x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 40, 0), Vector3(0, 2, 0), 0.9999, { 0.6,0.3,0.56,1 });
	x->addGenerator(_circleGen); x->setNumGen(0);
	x->setLifeTime(2.0);
	_fireworks_pool.push_back(x);
}
void ParticleSystem::update(double t) {
	for (auto pg : _particle_generators) {
		if (pg->getName() == "CHORRO") {
			if (pg->isOn()) {
				auto ts = pg->generateParticles();
				for (auto p_ : ts)_tornado.push_back(p_);
			}

		}
		else
		{
			if (pg->isOn()) {
				auto parts = pg->generateParticles();
				//	 pg->generateParticles(_particles);
				for (auto p_ : parts)_particles.push_back(p_);
			}
		}



	}
	ultimoGenerado -= t;
	if (salaEnemigos && ultimoGenerado <= 0) {
		createEnemies({ 300,0,0 });
		ultimoGenerado =timerEnemigos/2;
	}
	if (myPlayer->getGlobalPose().p.x >= tornadoCoor)
		changeSala();
	if (onTornado && abs(aux->getPosition().x) > 200) {
		playerNum = "REAL";
		movementAvailable = true;
		onTornado = false;
		_force_reg->deleteParticleRegistry(aux);
		chorroGauss->turnOff();
		_whirlwind->active = false;
		aux->clearForce();
		aux->setVelocity({ 0,0,0 });
		aux->setAcceleration({ 0,0,0 });
		aux->setPosition(myPlayer->getGlobalPose().p);
		canSlink = true;
	}


	updateParticles(t);
	updateEnemies(t);
	updateEnemyBullets(t);
	updateBullets(t);
	_force_reg->updateForce(t);
	_solid_reg->updateForce(t);
}

void ParticleSystem::setGravityEffect()
{
	for (auto& p : _particles)
		_force_reg->addRegistry(getForceGenerator("GRAV"), p);

}

void ParticleSystem::explosion()
{
	for (auto p : _particles) {
		_exp->setTime(GetLastTime());
		_force_reg->addRegistry(_exp, p);
	}
}

void ParticleSystem::turnOffAllForces()
{
	for (auto fg : _force_generators)
		fg->active = false;
}

void ParticleSystem::turnOnForce(std::string name)
{
	turnOffAllForces();
	getForceGenerator(name)->active = true;
}

void ParticleSystem::generateSpringDemo()
{
	//Particle* p1=new Particle()
	Particle* p1 = new Particle(FIREBALL, { -10,10,0 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	Particle* p2 = new Particle(FIREBALL, { 10,10,0 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });

	auto f1 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 10, p2));
	f1->_name = "SPRING1";
	//f1->active = true;
	auto f2 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 10, p1));
	_force_generators.push_back(f1);
	_force_generators.push_back(f2);
	f2->_name = "SPRING2";
	//f2->active = true;
	_force_reg->addRegistry(f1.get(), p1);

	_force_reg->addRegistry(f2.get(), p2);



	_particles.push_back(p1);
	_particles.push_back(p2);


	Particle* p3 = new Particle(FIREBALL, { -10,20,0 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.5,0.2,0.0,1.0 });


	auto f3 = std::shared_ptr<ForceGenerator>(new AnchoredSpringFG(1, 10, { 10.0,20.0,0.0 }));
	f3->_name = "ANCHO";
	_force_generators.push_back(f3);

	_force_reg->addRegistry(f3.get(), p3);
	_particles.push_back(p3);
}

void ParticleSystem::generateSlinkyDemo(std::string action)
{
	//Particle* p1 = new Particle(FIREBALL, { 0,35,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	//Particle* p2 = new Particle(FIREBALL, { 0,30,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	//Particle* p3 = new Particle(FIREBALL, { 0,25,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	//Particle* p4 = new Particle(FIREBALL, { 0,20,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	//Particle* p5 = new Particle(FIREBALL, { 0,15,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	//Particle* p6 = new Particle(FIREBALL, { 0,10,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	////particulas
	//if (action == "create") {

	//	p1 = new Particle(FIREBALL, { 0,35,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	//	p2 = new Particle(FIREBALL, { 0,30,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	//	p3 = new Particle(FIREBALL, { 0,25,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	//	p4 = new Particle(FIREBALL, { 0,20,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	//	p5 = new Particle(FIREBALL, { 0,15,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	//	p6 = new Particle(FIREBALL, { 0,10,-70 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	//	_particles.push_back(p1);
	//	_particles.push_back(p2);
	//	_particles.push_back(p3);
	//	_particles.push_back(p5);
	//	_particles.push_back(p4);
	//	_particles.push_back(p6);
	//}
	//else {
	//	auto f1 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 1, p1));
	//	_force_generators.push_back(f1);
	//	_force_reg->addRegistry(f1.get(), p2);

	//	auto f2 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 1, p2));
	//	_force_generators.push_back(f2);
	//	_force_reg->addRegistry(f2.get(), p3);
	//	auto f3 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 1, p3));
	//	_force_generators.push_back(f3);
	//	_force_reg->addRegistry(f3.get(), p4);

	//	auto f4 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 1, p4));
	//	_force_generators.push_back(f4);
	//	_force_reg->addRegistry(f4.get(), p5);
	//	auto f5 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(1, 1, p5));
	//	_force_generators.push_back(f5);
	//	_force_reg->addRegistry(f5.get(), p6);

	//	_force_reg->addRegistry(getForceGenerator("GRAV"), p2);
	//	_force_reg->addRegistry(getForceGenerator("GRAV"), p3);
	//	_force_reg->addRegistry(getForceGenerator("GRAV"), p4);
	//	_force_reg->addRegistry(getForceGenerator("GRAV"), p5);
	//	_force_reg->addRegistry(getForceGenerator("GRAV"), p6);
	//}


}

void ParticleSystem::generateSlinky()
{
	p1 = new Particle(FIREBALL, { 320,55,-20 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	p2 = new Particle(FIREBALL, { 320,50,-20 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });
	p3 = new Particle(FIREBALL, { 320,45,-20 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30.0, { 0.7,0.4,1.0,1.0 });


	_slinkyChain.push_back(p1);
	_slinkyChain.push_back(p2);
	_slinkyChain.push_back(p3);


}

void ParticleSystem::playerJoinsSlinky(AuxiliarPlayer* p, physx::PxRigidDynamic* pN)
{
	if (destroyedSlinky ||!canSlink) return;
	if (!p->isOnSlinky) {
		_force_reg->deleteParticleRegistry(p);
		p->setVelocity({ 0,0,0 });
		p->clearForce();
		p->savedPos = pN->getGlobalPose().p;
		/*	saveP = pN->getGlobalPose().p;*/
		p->setPosition({ 320,40,-20 });

		_force_reg->addRegistry(getForceGenerator("GRAV"), p);
		//_particles.push_back(p);
		for (auto s : _slinkyChain) {
			_particles.push_back(s);
		}

		auto f1 = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(11, 15, _slinkyChain.front()));
		_slinkyChain.pop_front();
		_force_generators.push_back(f1);
		_force_reg->addRegistry(f1.get(), _slinkyChain.front());

	

		auto fp = std::shared_ptr<ForceGenerator>(new SpringForceGenerator(15, 1, _slinkyChain.front()));

		

		_force_generators.push_back(fp);
		_force_reg->addRegistry(fp.get(), p);
		//_solid_reg->addRegistry(fp.get(), pN);

		_force_reg->addRegistry(getForceGenerator("GRAV"), _slinkyChain.front());
		p->isOnSlinky = true;

		playerNum = "AUX";

		
	}
	else {
		delete p1; delete p2;
		delete p3;
		_force_reg->deleteParticleRegistry(p);
		salaEnemigos = true;
		p->setVelocity({ 0,0,0 });
		p->setPosition(p->savedPos);
		p->isOnSlinky = false;
		destroyedSlinky = true;
		playerNum = "REAL";
		
	}

}

void ParticleSystem::generateBuoyancyDemo()
{
	Particle* p = new Particle(FIREBALL, { 10,60,0 }, { 0,0,0 }, { 0,0,0 }, 200.0, 0.99, 30.0, { 0.9,0.1,0.5,1.0 });
	_particles.push_back(p);

	auto b = std::shared_ptr<ForceGenerator>(new BuoyancyForceGenerator(10, 0.5, 1000));
	_force_generators.push_back(b);
	_force_reg->addRegistry(b.get(), p);

	_force_reg->addRegistry(getForceGenerator("GRAV"), p);
}

void ParticleSystem::moveGameCamera()
{
	Vector3 cameraPos;
	if (playerNum == "REAL") {
		cameraPos = myPlayer->getGlobalPose().p;
	}
	else  cameraPos = aux->getPosition();

	auto auxDir = GetCamera()->getDir();
	auxDir.y = 0;
	
	cameraPos.y += 20;

	cameraPos -= auxDir * 20;
	GetCamera()->mEye = cameraPos;
}

void ParticleSystem::movePlayer()
{
	if (!movementAvailable) return;

	myPlayer->setLinearVelocity({ GetCamera()->getDir().x * 30,0, GetCamera()->getDir().z * 30 });
}

void ParticleSystem::shootBullet(Vector3 posicion)
{
	if (!movementAvailable||_bullets.size()>=3) return;
	auto b = gPhysics->createRigidDynamic(physx::PxTransform({ posicion }));
	b->setLinearVelocity({ GetCamera()->getDir() * 200 });
	
	b->setAngularVelocity({ 0,0,0 });
	b->setName("Bullet");

	auto shape = CreateShape(physx::PxSphereGeometry(1.0)); b->attachShape(*shape);
	physx::PxRigidBodyExt::setMassAndUpdateInertia(*b, .5);

	auto x = new RenderItem(shape, b, { 0.5, 0.8, 0.7, 1.0 });
	b->setLinearDamping(0.99);

	gScene->addActor(*b);




	Solid* s = new Solid();
	s->act = b; s->_remaining_time = 2; s->rI = x; s->isAlive = true;
	_bullets.push_back(s);
}
void ParticleSystem::shootEnemy(Vector3 posicion)
{
	
	auto b = gPhysics->createRigidDynamic(physx::PxTransform({ posicion + Vector3(3,-1,0) }));
	b->setLinearVelocity({ -GetCamera()->getDir() * 200 });
	auto r1 = rand() % ((2 - (-2) +1))-2 ;
	auto r2 = 1;
	auto r3 = rand() % ((2 - (-2) + 1)) - 2;
	Vector3 v = { (float)r1 , (float)r2, (float)r3+3 };

	//b->setLinearVelocity(v*75);
	b->setAngularVelocity({ 0,0,0 });
	b->setName("BulletEnemy");

	auto shape = CreateShape(physx::PxSphereGeometry(2.0)); b->attachShape(*shape);
	physx::PxRigidBodyExt::setMassAndUpdateInertia(*b, 1.5);

	auto x = new RenderItem(shape, b, { 0.9, 0.2, 0.7, 1.0 });
	b->setLinearDamping(0.99);

	gScene->addActor(*b);




	Solid* s = new Solid();
	s->act = b; s->_remaining_time = 2; s->rI = x; s->isAlive = true;
	_bulletsEnemy.push_back(s);
}
bool ParticleSystem::isPlayerOnFloor()
{
	return false;/*physx::PxGeometryQuery::overlap(myPlayer->, (*s)->act->getGlobalPose(), e->rI->shape->getGeometry().box(), e->act->getGlobalPose());*/
}
void ParticleSystem::jumpPlayer()
{
	//if(isPlayerOnFloor())
	if (!movementAvailable) return;
	if (myPlayer->getGlobalPose().p.y <= 2.0)
		myPlayer->setLinearVelocity(myPlayer->getLinearVelocity() + Vector3(0, 10, 0));
}
void ParticleSystem::changeSala()
{
	aux->setPosition(myPlayer->getGlobalPose().p);
	playerNum = "AUX";

	movementAvailable = false;
	tornadoCoor += 350;
	myPlayer->setGlobalPose({ 300,2,0 });
	onTornado = true;
	_force_reg->addRegistry(getForceGenerator("WHIRLWIND"), aux);
}
void ParticleSystem::createEnemies(Vector3 pos)
{
	/*if (num_enemies < 20) {*/
		auto r = rand() % 5 + 1;
		Vector3 size = { (float)r,(float)r, (float)r };

		auto posi = Vector3(pos.x + rand() % ((90 - -90) + 1) + -90, pos.y + rand() % 5, pos.z + rand() % ((40 - -40) + 1) + -40);//posicion dentro de la caja
		physx::PxRigidDynamic* solid = gPhysics->createRigidDynamic(physx::PxTransform({ posi }));
		solid->setLinearVelocity({ 20,0,0 });
		solid->setAngularVelocity({ 0, 0, 0 });


		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(size));
		solid->attachShape(*shape);
		solid->setName("Enemy");
		solid->setMassSpaceInertiaTensor({ size.y * size.z , size.x * size.z, size.x * size.y });

		auto r2 = rand() % 255 + 0;
		auto g = rand() % 255 + 0;
		auto b = rand() % 255 + 0;
		auto x = new RenderItem(shape, solid, { (float)0, (float)g / 255, (float)b / 255, 1 });
		gScene->addActor(*solid);

		Solid* s = new Solid();
		s->act = solid; s->_remaining_time = 10; s->rI = x; s->isAlive = true;


		_enemies.push_back(s);

		/*if (_whirlwind->active)
			_solid_reg->addRegistry(getForceGenerator("WHIRLWIND"), solid);*/
		if (_wind->active)
			_solid_reg->addRegistry(getForceGenerator("WIND"), solid);
	//}

}
//Metodo para borrar solidos
void ParticleSystem::releaseSolid(Solid* s)
{
	if (s->isAlive) {
		s->isAlive = false;
		_solid_reg->deleteParticleRegistry(static_cast<physx::PxRigidDynamic*> (s->act));
		s->act->release();
		DeregisterRenderItem(s->rI);
		//s->rI->color
		delete s;
	}

}
//Metodo para actualizar solidos
void ParticleSystem::updateBullets(double t)
{
	auto s = _bullets.begin();
	while (s != _bullets.end()) {
		if ((*s)->isAlive) {
			for (auto e : _enemies) {

				if (physx::PxGeometryQuery::overlap((*s)->rI->shape->getGeometry().sphere(), (*s)->act->getGlobalPose(), e->rI->shape->getGeometry().box(), e->act->getGlobalPose())) {
					auto e2 = static_cast<physx::PxRigidDynamic*>(e->act);
					_solid_reg->addRegistry(getForceGenerator("WIND"), e2);
					colorSolid(e, { 1.0,0.0,0.0,1.0 });
				}
			}
			(*s)->_remaining_time -= t;
			if ((*s)->_remaining_time <= 0) {
				releaseSolid(*s);
				s = _bullets.erase(s);
			}
			else
				s++;

		}

	}
}
void ParticleSystem::updateEnemies(double t)
{
	auto e = _enemies.begin();
	while (e != _enemies.end()) {
		if ((*e)->isAlive) {
			(*e)->ultimoDisparo -= t;
			if ((*e)->ultimoDisparo <= 0) {
				shootEnemy((*e)->act->getGlobalPose().p);
				(*e)->ultimoDisparo = 50;
			}
			(*e)->_remaining_time -= t;
			if ((*e)->_remaining_time <= 0) {
				releaseSolid(*e);
				//colorSolid(*s, { 1.0,0.0,0.0,1.0 });
				e = _enemies.erase(e);
			}
			else
				e++;


		}
	}
}
void ParticleSystem::updateEnemyBullets(double t)
{
	auto e = _bulletsEnemy.begin();
	while (e != _bulletsEnemy.end()) {
		if ((*e)->isAlive) {
			(*e)->_remaining_time -= t;
			if ((*e)->_remaining_time <= 0) {
				releaseSolid(*e);
				//colorSolid(*s, { 1.0,0.0,0.0,1.0 });
				e = _bulletsEnemy.erase(e);
			}
			else
				e++;


		}
	}
}
//Metodo para cambiar de color solidos
void ParticleSystem::colorSolid(Solid* s, Vector4 c)
{
	s->rI->color = c;
}


void ParticleSystem::createSalas() {
	auto desplX = 0;

	for (int i = 0; i < 2; i++)
	{
		physx::PxRigidStatic* floor = gPhysics->createRigidStatic(physx::PxTransform({ 0.0f + desplX, 0.0f, .0f }));
		physx::PxMaterial* material = gPhysics->createMaterial(0.5, 0.5, 0.3);
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*floor, physx::PxBoxGeometry(100.0, 1.0, 50.0), *material);
		floor->setName("Suelo");
		auto itemFloor = new RenderItem(shape, floor, { 0.0, 0.42, 0.24, 1.0 });
		gScene->addActor(*floor);
		Solid* s = new Solid();
		s->act = floor; s->rI = itemFloor;
		_statics.push_back(s);

		// Paredes
		physx::PxRigidStatic* wall = gPhysics->createRigidStatic(physx::PxTransform({ 100.0f + desplX, 0.0f, 0.0f }));
		shape = CreateShape(physx::PxBoxGeometry(1, 10, 50));
		wall->attachShape(*shape);
		auto itemWall = new RenderItem(shape, wall, { 0.4, 0.2, 0.6, 1.0 });
		gScene->addActor(*wall);
		Solid* s2 = new Solid();
		s2->act = wall; s2->rI = itemWall;
		_statics.push_back(s2);


		wall = gPhysics->createRigidStatic(physx::PxTransform({ -100.0f + desplX, 0.0f, .0f }));
		wall->attachShape(*shape);
		itemWall = new RenderItem(shape, wall, { 0.4, 0.2, 0.6, 1.0 });
		gScene->addActor(*wall);
		Solid* s3 = new Solid();
		s3->act = wall; s3->rI = itemWall;
		_statics.push_back(s3);

		wall = gPhysics->createRigidStatic(physx::PxTransform({ 0.0f + desplX, 0.0f, -50.0f }));
		shape = CreateShape(physx::PxBoxGeometry(100, 10, 1));
		wall->attachShape(*shape);
		itemWall = new RenderItem(shape, wall, { 0.4, 0.2, 0.6, 1.0 });
		gScene->addActor(*wall);
		Solid* s4 = new Solid();
		s4->act = wall; s4->rI = itemWall;
		_statics.push_back(s4);

		wall = gPhysics->createRigidStatic(physx::PxTransform({ 0.0f + desplX, 0.0f, 50.0f }));
		wall->attachShape(*shape);
		itemWall = new RenderItem(shape, wall, { 0.4, 0.2, 0.6, 1.0 });
		gScene->addActor(*wall);
		//creacion del slinky
		Solid* s5 = new Solid();
		s5->act = wall; s5->rI = itemWall;
		_statics.push_back(s5);

		desplX += 300;

	}
	//base canasta
	auto wall = gPhysics->createRigidStatic(physx::PxTransform({ 21.8f, 0.0f, .0f }));
	auto shape = CreateShape(physx::PxBoxGeometry(1, 20, 1));
	wall->attachShape(*shape);
	auto itemWall = new RenderItem(shape, wall, { 1, 0.2, 0.8, 1.0 });
	gScene->addActor(*wall);
	Solid* s6 = new Solid();
	s6->act = wall; s6->rI = itemWall;
	_statics.push_back(s6);
	//tablero
	wall = gPhysics->createRigidStatic(physx::PxTransform({ 20.0f, 17.0f, 00.0f }));

	shape = CreateShape(physx::PxBoxGeometry(1, 3, 5));
	wall->attachShape(*shape);
	itemWall = new RenderItem(shape, wall, { 0.2, 0.2, 0.2, 1.0 });
	gScene->addActor(*wall);
	Solid* s7 = new Solid();
	s7->act = wall; s7->rI = itemWall;
	_statics.push_back(s7);
	//cesto

	wall = gPhysics->createRigidStatic(physx::PxTransform({ -2.0f + 20, 14.0f, 0.0f }));
	shape = CreateShape(physx::PxBoxGeometry(1.5, 1.5, 1));
	wall->attachShape(*shape);
	itemWall = new RenderItem(shape, wall, { 0.0, 0.2, 0.7, 1.0 });
	gScene->addActor(*wall);
	Solid* s8 = new Solid();
	s8->act = wall; s8->rI = itemWall;
	_statics.push_back(s8);
	wall = gPhysics->createRigidStatic(physx::PxTransform({ -2.0f + 20, 15.5f, 0.0f }));
	shape = CreateShape(physx::PxBoxGeometry(1.5, 0.1, 1));
	wall->attachShape(*shape);
	wall->setName("Canasta");
	itemWall = new RenderItem(shape, wall, { 0.0, 0.2, 0.7, 1.0 });
	gScene->addActor(*wall);
	Solid* s9 = new Solid();
	s9->act = wall; s9->rI = itemWall;
	_statics.push_back(s9);

}