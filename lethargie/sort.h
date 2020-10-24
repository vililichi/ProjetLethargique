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
	std::vector<float> alpha0;
	bool alpha0Set;


public:
	std::vector<Vivant*> blacklist;
	std::vector<Vivant*> whitelist;
	float endTime;
	bool whiteliste_only;
	bool percing;
	bool spectral;
	Damage dommage;

	Sort();
	float get_lifeTime();
	float set_lifeTime(float new_lifeTime);
	void update(float sec);
	void collideWall(RigidBody& wall);
	void collideVivant(Vivant& cible);
};

Sort* importSort(std::string path, float life_time = -1, bool is_floating = false, bool is_spectral = false, bool has_percing = false, bool is_dynamic = true);
Sort* SetActorAndWorld(Sort* p_sort, Vivant* acteur, bool blacklist_player = true);
