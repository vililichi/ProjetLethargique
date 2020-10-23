#pragma once

#include "Physic.h"
#include "Joueur.h"
#include "Sort.h"

class Monde
{
protected:
	sf::Clock temps;
public:
	std::vector<VisibleBody*> dynamiques;
	std::vector<VisibleBody*> statiques;
	std::vector<Joueur*> joueurs;
	std::vector<Vivant*> vivants;
	std::vector<Sort*> sorts;

	Float2 gravity;

	Monde();
	~Monde();
	void clear();
	VisibleBody* addStatique(VisibleBody);
	VisibleBody* addDynamique(VisibleBody);
	Joueur* addJoueur();
	Vivant* addVivant(Vivant*);
	Sort* addSort( Sort);
	Sort* addSort(Sort*);

	void update();
	void afficher(sf::RenderWindow& fenetre);

};
