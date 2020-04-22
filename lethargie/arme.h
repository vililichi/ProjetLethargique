#pragma once
#include "Vivant.h"

class Joueur;

class arme
{
	friend Competence;
public:
	Competence* competence1;
	Competence* competence2;
	sf::Sprite apparence;
};

class Competence
{
public:
	Damage degat;
	virtual bool action(Float2 position, Float2  objectif, Joueur acteur) = 0;
};


#pragma region Infusion

class InfusionSphere : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

class InfusionDart : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

class InfusionNova : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

class InfusionExplosion : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

#pragma endregion

#pragma region aiguille
class AiguillePercante : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

class AiguilleTaillante : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

class AiguilleCirculaire : public Competence
{
	bool action(Float2 position, Float2  objectif, Joueur acteur);

};

#pragma endregion
