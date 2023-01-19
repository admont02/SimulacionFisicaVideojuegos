#pragma once
#include "Particle.h"
#include <list>
#include <memory>
#include "particleGenerator.hpp"
using namespace std;

class Firework :public Particle {
	list<shared_ptr<ParticleGenerator>> gens;
	int numGen = 0;
	
public:
	Firework();
	~Firework() {
		while (!gens.empty())
			gens.pop_front();
	}
	Firework(Vector3 p, Vector3 v, Vector3 acc, float d, Vector4 col ):Particle(FIREWORK, p,  v,  acc, 0.5,  d,3.0,  col) {

	}
	Firework* clone()const override {
		Firework* newRK = new Firework(pos, vel, a, damping, color);
		newRK->setLifeTime(2.0); newRK->setNumGen(numGen);
		for (auto it : gens)newRK->addGenerator(it);
		return newRK;
	}
	//void update(double t) override;
	void explode(list<Particle*>& list) {
		for (auto it : gens) {
			

			if (getNumGen() + 1 <  4) {
				auto pt = clone();
				pt->setNumGen(numGen + 1);
				pt->setLifeTime(2.0);
				auto r = rand() % 255 + 0;
				auto g = rand() % 255 + 0;
				auto b = rand() % 255 + 0;
				pt->setColor(Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0));
				pt->setAcceleration(Vector3(0, -10 / (numGen + 1), 0));
				pt->setVelocity(pt->getVelocity());


				(*it).setNumPart(10);
				(*it).setParticle(pt);
				(*it).generateParticles(list);
				pt->setPosition(Vector3(1000000, 100000000, 100000));
			}
				
			
		}
	}
	void addGenerator(shared_ptr<ParticleGenerator> gn) {
		gens.push_back(gn); 
	}
	void deleteGens() {
		while (!gens.empty())gens.pop_front(); 
	}
	void setNumGen(int x) { 
		numGen = x; 
	}
	int getNumGen() { 
		return numGen; 
	};
};