#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

enum ParticleType {
	FIREBALL
};
class Particle {
private:
	Vector3 vel;
	Vector3 pos;
	RenderItem* renderItem;
	physx::PxTransform pose;//A render item le pasamos dir de este pose,para que se actualice
	Vector3 a;
	float damping;
	float mass;
	ParticleType _type;
	Vector3 dir;

	double _remaining_time;
public:
	Particle() {};
	Particle(ParticleType Pt);
	~Particle();

	void integrate(double t);

	//metodos para setear atributos
	inline void setMass(float M) { mass = M; }
	inline void setVelocity(double Vel) {
		vel = Vel * dir;
	}
	inline void setAcceleration(Vector3 Acc) { a = Acc; }
	inline void setDamping(float D) { damping = D; }
	inline void setPosition(Vector3 Pos) { pos = Pos; pose = physx::PxTransform(pos); }
	inline Vector3 getPosition() { return pos; }
	void establishParticle();


};
#endif