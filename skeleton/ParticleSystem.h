#pragma once
#include "Particle.h"


#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <vector>
#include <list>
#include "./forces/ForceGenerator.hpp"
#include "./forces/ParticleForceRegistry.hpp"
#include "Player.h"
#include "Firework.h"
#include "CircleGenerator.h"
#include "GaussianParticleGenerator.hpp"

enum GeneratorType {
	GAUSSIAN, UNIFORM
};
struct Solid {
	RenderItem* rI;
	physx::PxRigidActor* act;
	double _remaining_time = 5;
	bool isAlive = false;
	int ultimoDisparo = 300;
	
};
class ParticleSystem {
protected:

	int tornadoCoor = 0;
	physx::PxRigidDynamic* myPlayer;
	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;
	Vector3 vel;
	RenderItem* renderItem;
	physx::PxTransform pose;
	Particle* particle;
	Particle* p1;
	Particle* p2;
	Particle* p3;
	double damping;
	std::list<Particle*>_particles;
	std::list<Particle*>_tornado;

	std::list<Particle*>_slinkyChain;
	std::list<std::shared_ptr<ParticleGenerator>>_particle_generators;
	
	GaussianParticleGenerator* chorroGauss;
	std::shared_ptr<ForceGenerator>_grav;
	std::shared_ptr<ForceGenerator>_wind;
	std::shared_ptr<ForceGenerator>_whirlwind;
	std::shared_ptr<ParticleGenerator> _circleGen;
	ExplosionForceGenerator* _exp;
	AuxiliarPlayer* aux;
	std::vector<Firework*> _fireworks_pool;

	ParticleForceRegistry* _force_reg;
	SolidForceRegistry* _solid_reg;
	std::list<std::shared_ptr<ForceGenerator>> _force_generators;
	SpringForceGenerator* springF;
	bool isSlinkyActive = false;
	bool destroyedSlinky = false;
	bool movementAvailable;
	bool canSlink = false;

public:
	ParticleSystem(physx::PxRigidDynamic* realPlayer, AuxiliarPlayer* p, physx::PxScene* scene, physx::PxPhysics* physics);
	~ParticleSystem();

	void shootParticle(ParticleType t);
	void updateParticles(double t);
	void generateFireworkSystem();
	void update(double t);
	void addGenerator(GeneratorType type);
	
	ForceGenerator* getForceGenerator(std::string name);
	void activeParticleGenerator(GeneratorType gt);
	//firework
	void shootFirework(int type);
	void onParticleDeath(Particle* p);

	void setGravityEffect();
	void explosion();
	void turnOffAllForces();
	void turnOnForce(std::string name);
	void generateSpringDemo();
	void generateSlinkyDemo(std::string action);
	void generateSlinky();
	void playerJoinsSlinky(AuxiliarPlayer* p, physx::PxRigidDynamic* p2);
	void generateBuoyancyDemo();
	//aumentar/disminuir k
	inline void increaseKSpring() {
		auto a = dynamic_cast<AnchoredSpringFG*>(getForceGenerator("ANCHO"));
		a->setk(a->getK() + 15.0);
	}
	inline void decreaseKSpring() {
		auto a = dynamic_cast<AnchoredSpringFG*>(getForceGenerator("ANCHO"));
		a->setk(a->getK() - 15.0);
	}
	Vector3 saveP;


	//camara
	bool onTornado = false;
	string playerNum = "REAL";
	void moveGameCamera();	
	//player
	void movePlayer();
	void shootBullet(Vector3 posicion);
	void shootEnemy(Vector3 posicion);

	bool isPlayerOnFloor();
	void jumpPlayer();
	void changeSala();
	//creacion/destruccion de solidos
	std::list<Solid*> _bullets;
	std::list<Solid*> _bulletsEnemy;

	std::list<Solid*> _enemies;
	std::list<Solid*> _statics;

	int num_enemies;
	void createSalas();

	//timer enemigos
	int timerEnemigos = 300;
	int ultimoGenerado = 0;
	bool salaEnemigos = false;
	void createEnemies(Vector3 pos);
	void releaseSolid(Solid* s);
	void updateBullets(double t);
	void updateEnemies(double t);
	void updateEnemyBullets(double t);

	void colorSolid(Solid* s, Vector4 c);
	Vector3 getPlayerPos() { return myPlayer->getGlobalPose().p; }
};
