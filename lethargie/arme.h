#pragma once
#include "vivant.h"

class Joueur;
class Arme;

class Competence
{
	friend Arme;
protected:
	Joueur* acteur;
	Float2 position, objectif;
	int etape;
public:
	Competence();

	float animDuree;
	float animEtat;
	bool animActif;

	Damage degat;
	Damage cout;

	void update(float sec);
	void activate();
	virtual void anim(float sec) = 0;
};

class Arme
{
protected:
	Competence* competence1;
	Competence* competence2;
	Joueur* possesseur;
public:
	Arme();
	~Arme();

	sf::Sprite apparence;

	void setCompetence(Competence* competence, short noCompetence);
	void update(float sec);
	void bound(Joueur* _possesseur);
};




#pragma region Infusion

class InfusionSphere : public Competence
{
	void anim(float sec);
};

class InfusionDart : public Competence
{
	void anim(float sec);
};

class InfusionNova : public Competence
{
	void anim(float sec);
};

class InfusionExplosion : public Competence
{
	void anim(float sec);
};

#pragma endregion

#pragma region aiguille
class AiguillePercante : public Competence
{
	void anim(float sec);
};

class AiguilleTaillante : public Competence
{
	void anim(float sec);
};

class AiguilleCirculaire : public Competence
{
	void anim(float sec);
};

class AiguilleDistante : public Competence
{
	void anim(float sec);
};

#pragma endregion
