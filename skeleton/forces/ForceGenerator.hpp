#pragma once
#include <random>
#include <list>
#include <iostream>
#include "../core.hpp"
#include "../Particle.h"

class ForceGenerator {
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	virtual void updateForce(physx::PxRigidBody* solid, double duration){}
	
	std::string _name;
	double t = -1e10;//if starting negative --> eternal
	bool active = false;
};



class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& g) { _gravity = g; }
	virtual void updateForce(Particle* p, double t) {
		if (fabs(p->getInvMass()) < 1e-10) return;
		p->addForce(_gravity * p->getMass());
	}
	virtual void updateForce(physx::PxRigidBody* solid, double duration) {
		if (fabs(solid->getInvMass()) < 1e-10) return;
		solid->addForce(_gravity * solid->getMass());
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
	ParticleDragGenerator(const float k1, const float k2) {
		_k1 = k1;
		_k2 = k2;
	}

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
class WindForceGenerator : public ParticleDragGenerator {
protected:
	Vector3 vel;
public:
	WindForceGenerator();
	WindForceGenerator(const float k1, const float k2, Vector3 _vel);
	void updateForce(Particle* particle, double t) override;
	virtual void updateForce(physx::PxRigidBody* solid, double duration);

};
class WhirlwindForceGenerator : public WindForceGenerator {
protected:
	float k;
	Vector3 c;
public:
	WhirlwindForceGenerator();
	WhirlwindForceGenerator(const float _k, Vector3 _vel, Vector3 _cen) : WindForceGenerator(_k, 0, _vel) {
		k = _k;
		c = _cen;
	}
	void updateForce(Particle* particle, double t) override {
		if (fabs(particle->getInvMass()) < 1e-10) return;

		auto p = particle->getPosition();
		vel = { k * (-(p.z - c.z)),k * (35 - (p.y - c.y)),k * (p.x - c.x) };

		Vector3 v = particle->getVelocity() - vel;
		float drag_coef = v.normalize();
		Vector3 dragF;
		drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
		dragF = -v * drag_coef;
		particle->addForce(dragF);

	}
};
class ExplosionForceGenerator : public ForceGenerator {
protected:
	float _k;
	Vector3 _p;
	double _R;
	double _R0;

	Vector3 _f;
	float _m = 30.0f;


public:
	float _time;
	ExplosionForceGenerator();
	ExplosionForceGenerator(Vector3 pos, float k, double R) {
		_p = pos;
		_k = k;
		_R = R;
		_R0 = R;
		_f = { 0,0,0 };
	}
	void updateForce(Particle* particle, double t) override {
		if (fabs(particle->getInvMass()) < 1e-10) return;

		auto t1 = GetLastTime() - _time;
		if (_time <= _m)_R += 343 * _time;

		auto p = particle->getPosition();

		auto r = sqrt(pow((p.x - _p.x), 2) + pow((p.y - _p.y), 2) + pow((p.z - _p.z), 2));

		if (r < _R)
		{
			auto a = _k / pow(r, 2);
			auto b = pow(2.71828182846, -(t / _m));

			_f = Vector3(p.x - _p.x, p.y - _p.y, p.z - _p.z);
			_f *= a * b;
		}


		//std::cout << dragF.x << "/t" << dragF.y << "/t" << dragF.z << "/t" << std::endl;
		particle->addForce(_f);
		_R = _R0 + 343 * t1;
	}

	void setTime(double t) { _time = t; }
};

class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double resting_length, Particle* other) {
		_k = k;
		_resting_length = resting_length;
		_other = other;
	}
	virtual void updateForce(Particle* particle, double t) {
		Vector3 force = _other->getPosition() - particle->getPosition();

		const float length = force.normalize();
		const float delta_x = length - _resting_length;

		force *= delta_x * _k;

		particle->addForce(force);
	}

	inline void setk(double k) { _k = k; };

	inline double getK() { return _k; }

	~SpringForceGenerator() = default;

protected:
	double _k;
	double _resting_length;
	Particle* _other;

};
class AnchoredSpringFG : public SpringForceGenerator{
public:
	AnchoredSpringFG(double k, double resting, Vector3 anchor_pos): SpringForceGenerator(k, resting, nullptr)
	{
		_other = new Particle(BOX,anchor_pos, { 0,0,0 }, { 0, 0, 0 },1e6,0.9,30.0, { .8, 0.6, 0.0, 1.0 });
	}

	~AnchoredSpringFG() {
		delete _other;
	}
};

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float V, float d) {
		_height = h;
		_volume = V;
		_liquid_density = d;

		_liquid_particle = new Particle(PLANE, { 0,45,0 }, { 0,0,0 }, { 0,0,0 }, 2.0, 0.99, 30000.0, { 0.5,0.8,.7,1.0 });
	}

	virtual void updateForce(Particle* particle, double t) {
		float h = particle->getPosition().y;
		float h0 = _liquid_particle->getPosition().y;

		Vector3 f(0, 0, 0);
		float immersed = 0.0;

		if (h - h0 > _height * 0.5)
		{
			immersed = 0.0;
		}
		else if (h0 - h > _height * 0.5) {
			immersed = 1.0;
		}
		else
		{
			immersed = (h0 - h) / _height + 0.5;
		}

		f.y = _liquid_density * _volume * immersed * 9.8;

		particle->addForce(f);
	}

	virtual ~BuoyancyForceGenerator() = default;

protected:

	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle;
};