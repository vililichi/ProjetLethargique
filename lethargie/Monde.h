#pragma once

#include "Physic.h"
#include "Joueur.h"

class Monde
{
protected:
	sf::Clock temps;
public:
	std::vector<corps_visible*> dynamiques;
	std::vector<corps_visible*> statiques;
	std::vector<Joueur*> joueurs;

	Float2 gravity;

	Monde();
	~Monde();
	void clear();
	corps_visible* addStatique(corps_visible);
	corps_visible* addDynamique(corps_visible);
	Joueur* addJoueur();

	void update();
	void afficher(sf::RenderWindow& fenetre);

};
