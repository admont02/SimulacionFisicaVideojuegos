#pragma once

#include "particleGenerator.hpp"
#include <random>

using namespace std;

class CircleGenerator : public ParticleGenerator
{
	
	int num;
	const float PI = 3.1415;
public:
	CircleGenerator() {
		
		num=10;
		_name = "CIRCLE"; 
	}
	~CircleGenerator() = default;

	void generateParticles(list<Particle*>& l) override {
		auto random = rand() % 30 + 10;
		auto angle = 90;
		for (int i = 0; i < num; i++)
		{
			auto p = _model->clone();
			
			p->setPosition(_model->getPosition());
			p->setVelocity(Vector3(_model->getVelocity().x + random * cos(angle * PI / 180.0),
				_model->getVelocity().y, _model->getVelocity().z + random * sin(angle * PI / 180.0)));
			angle += 360.0 / num;

			l.push_back(p);
		}
	}
	virtual std::list<Particle*>generateParticles() { std::list<Particle*>x; return x; }

};