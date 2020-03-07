#pragma once
#include "Vivant.h"


const int nbrAction = 5;

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
protected:
	float actionTimer[nbrAction];
	bool actionPret[nbrAction];
public :
	Joueur();
	controlInfo actionFutur;
	virtual void update(sf::Time deltaT);
	virtual infoColl operator + (corps& c);


};

class Controler
{ 
public:
	Joueur* p_joueur;

	Controler();
	void creerControl();
};