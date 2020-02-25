#pragma once

#include "Physic.h"

class Monde
{
protected:
	sf::Clock temps;
public:
	std::vector<corps_visible*> dynamiques;
	std::vector<corps_visible*> statiques;

	Monde();
	~Monde();
	void clear();
	corps_visible* addStatique(corps_visible);
	corps_visible* addDynamique(corps_visible);

	void update();
	void afficher(sf::RenderWindow& fenetre);

};
