#pragma once
#include "Joueur.h"
#include "FichierIO.h"

class HUD
{
protected:
	sf::Texture* ombre;
	sf::Texture* lumiere;
	sf::Texture* vide;

	sf::Sprite* ombres;
	sf::Sprite* lumieres;

	unsigned short nbr_lum;
	unsigned short nbr_omb;
	unsigned short act_lum;
	unsigned short act_omb;

	sf::Vector2f taille;

public:
	HUD();
	~HUD();
	Joueur* p_joueur;

	void afficher(sf::RenderWindow& fenetre);


};