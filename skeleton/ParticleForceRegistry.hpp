#ifndef __PART_FORCE_REG_H__
#define __PART_FORCE_REG_H__
#include "ForceGenerator.hpp"
#include <map>

typedef std::pair<ForceGenerator*, Particle*> FRPair;


class ParticleForceRegistry: public std::multimap<ForceGenerator*,Particle*>{
public:
	void updateForce(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	void addRegistry(ForceGenerator* fg, Particle* p) {
		this->insert(FRPair(fg, p));
	}
	void deleteParticleRegistry(Particle* p) {
		auto it = begin();
		while (it != end()) {
			if (it->second != p) it++;
			else it = this->erase(it);
		}
	}
};
#endif