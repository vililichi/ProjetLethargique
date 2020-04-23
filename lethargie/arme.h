#pragma once
#include "Vivant.h"

class Joueur;

class Competence
{
public:
	Competence();

	float animDuree;
	float animEtat;
	bool animActif;

	Damage degat;
	Float2 position, objectif;
	Joueur* acteur;

	void update(float sec);
	virtual bool anim(float sec) = 0;
};

class Arme
{
	friend Competence;
protected:
	Competence* competence1;
	Competence* competence2;
public:
	Arme();
	~Arme();

	sf::Sprite apparence;

	void setCompetence(Competence* competence, short noCompetence);
	void update(float sec);
};


#pragma region Infusion

class InfusionSphere : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

class InfusionDart : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

class InfusionNova : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

class InfusionExplosion : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

#pragma endregion

#pragma region aiguille
class AiguillePercante : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

class AiguilleTaillante : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

class AiguilleCirculaire : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

class AiguilleDistante : public Competence
{
	bool anim(Float2 position, Float2  objectif, Joueur& acteur, float sec);

};

#pragma endregion
