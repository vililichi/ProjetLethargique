#pragma once

#include "sort.h"
#include "FichierIO.h"

class Cherubin : public Vivant
{
protected:
	Vivant* cible;
	void IA(float sec);
	void animate(float sec);
	float idleState;
	float attackState;
	float timerIA;

public:
	Cherubin();
	void update(float sec);
	void attack(Damage description);
};