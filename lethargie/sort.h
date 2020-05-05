#pragma once
#include "Vivant.h"

class Sort : public VisibleBody
{
protected:

	float lifeTime;
	bool has_lifeTime;
	bool end;
	float endAnim;
	virtual void ending(float sec);


public:
	std::vector<Vivant*> blacklist;
	std::vector<Vivant*> whitelist;
	float endTime;
	bool whiteliste_only;
	bool percing;
	bool floating;
	bool spectral;
	Damage dommage;

	Sort();
	float get_lifeTime();
	float set_lifeTime(float new_lifeTime);
	void update(float sec);
	void collideWall(RigidBody& wall);
	void collideVivant(Vivant& cible);
};
