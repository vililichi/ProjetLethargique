#pragma once

#include "Physic.h"
#include "Joueur.h"

class Monde
{
protected:
	sf::Clock temps;
public:
	std::vector<VisibleBody*> dynamiques;
	std::vector<VisibleBody*> statiques;
	std::vector<Joueur*> joueurs;

	Float2 gravity;

	Monde();
	~Monde();
	void clear();
	VisibleBody* addStatique(VisibleBody);
	VisibleBody* addDynamique(VisibleBody);
	Joueur* addJoueur();

	void update();
	void afficher(sf::RenderWindow& fenetre);

};
