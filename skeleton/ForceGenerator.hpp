#ifndef __FORCE_GENERATOR_H__
#define __FORCE_GENERATOR_H__
#include "particle.hpp"
#include <random>
#include <list>
#include <iostream>


class ForceGenerator {
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string _name;
	double t = -1e10;//if starting negative --> eternal
};
#endif
#pragma once


class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& g) { _gravity = g; }
	virtual void updateForce(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		p->addForce(_gravity * p->getMass());
	}
	inline void setGravity(Vector3 g) {
		_gravity = g;
	}
protected:
	Vector3 _gravity;

};

#pragma once


class ParticleDragGenerator : public ForceGenerator {
public:
	ParticleDragGenerator();
	ParticleDragGenerator(const float k1, const float k2);

	virtual void updateForce(Particle* particle, double t) {
		if (fabs(particle->getInvMass()) < 1e-10) return;
		//compute drag force
		Vector3 v = particle->getVelocity();
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;
		//Apply drag force
		std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;
		particle->addForce(dragF);
	}
	inline void setDrag(float k1, float k2) {
		_k1 = k1;
		_k2 = k2;
	}
	inline float getK1() { return _k1; }
	inline float getK2() { return _k2; }
protected:
	float _k1;
	float _k2;
};