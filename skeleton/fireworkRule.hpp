#ifndef __FIREWORK_RULE_H__
#define __FIREWORK_RULE_H__
#include "particle.hpp"
#include <memory>
#include <list>
#include "particleGenerator.hpp"

struct Payload
{
	unsigned type;
	unsigned count;

	Payload(unsigned t, unsigned c) : type(t), count(c) {};

	void set(unsigned t, unsigned c) { type = t; count = c; };
};

struct FireworkRule
{
	unsigned _type;
	float _minAge;
	float _maxAge;

	Vector3 _minVelocity;
	Vector3 _maxVelocity;
	float _damping;

	std::vector<Payload> _payloads;

	void set(unsigned type, float minAge, float maxAge, Vector3 minVelocity, Vector3 maxVelocity, float damping, std::vector<Payload> payloads)
	{
		_type = type;
		_minAge = minAge;
		_maxAge = maxAge;
		_minVelocity = minVelocity;
		_maxVelocity = maxVelocity;
		_damping = damping;
		_payloads = payloads;
	};
};
#endif