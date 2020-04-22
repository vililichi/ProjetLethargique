#pragma once
#include "Vivant.h"
#include "arme.h"

const int nbrAction = 5;
const int nbrAnim = 3;

struct controlInfo
{
	bool jump = false;
	bool haut = false;
	bool bas = false;
	bool droite = false;
	bool gauche = false;
	bool dash = false;
	bool action1 = false;
	bool action2 = false;
};


class Joueur : public Vivant
{
	friend Competence;
protected:
	float actionTimer[nbrAction];
	bool actionPret[nbrAction];
	float animEtat[nbrAnim];
	bool animActif[nbrAnim];
	void updateAnim(float sec);
	bool swap(bool invers);
	bool isInvers;
	float tempsChute;

public :
	Joueur();
	controlInfo actionFutur;
	void update(sf::Time deltaT);
	bool collideJump(infoColl);
	Competence* arme;
};

class Controler
{ 
public:
	Joueur* p_joueur;

	Controler();
	void creerControl();
};