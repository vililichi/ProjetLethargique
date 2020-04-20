#include "Monde.h"

Monde::Monde()
{
	clear();
	temps.restart();
	gravity = Float2(0, 0);
}
Monde::~Monde()
{
	clear();
}
void Monde::clear()
{
	int tailleS = statiques.size();
	int tailleD = dynamiques.size();
	for (int i = 0; i < tailleS; i++)
	{
		if (statiques[i] != NULL)
		{
			delete statiques[i];
		}
	}
	for (int i = 0; i < tailleD; i++)
	{
		if (dynamiques[i] != NULL)
		{
			delete dynamiques[i];
		}
	}
	for (int i = 0, taille = joueurs.size(); i < taille; i++)
	{
		if (joueurs[i] != NULL)
		{
			delete joueurs[i];
		}
	}
	statiques.clear();
	dynamiques.clear();
	joueurs.clear();
}
VisibleBody* Monde::addStatique(VisibleBody new_corps)
{
	VisibleBody* p_new_corps = new VisibleBody(new_corps);
	p_new_corps->is_Dynamic = false;
	p_new_corps->setMonde(this);
	statiques.push_back(p_new_corps);
	return p_new_corps;
}
VisibleBody* Monde::addDynamique(VisibleBody new_corps)
{
	VisibleBody* p_new_corps = new VisibleBody(new_corps);
	p_new_corps->is_Dynamic = true;
	p_new_corps->setMonde(this);
	dynamiques.push_back(p_new_corps);
	return p_new_corps;
}
Joueur* Monde::addJoueur()
{
	Joueur* p_new_joueur = new Joueur();
	joueurs.push_back(p_new_joueur);
	p_new_joueur->setMonde(this);
	return p_new_joueur;
}
void Monde::update()
{
//nettoyage des pointeur null et calcul tailles

	//tailles initiale
	int tailleS = statiques.size();
	int tailleD = dynamiques.size();
	int tailleJ = joueurs.size();

	//statique
	for (int i = 0; i < tailleS;)
	{
		if (!statiques[i])
		{
			tailleS--;
			std::swap(statiques[i], statiques[tailleS]);
			statiques.pop_back();
		}
		else i++;
	}
	//dynamique
	for (int i = 0; i < tailleD;)
	{
		if (!dynamiques[i])
		{
			tailleD--;
			std::swap(dynamiques[i], dynamiques[tailleD]);
			dynamiques.pop_back();
		}
		else i++;
	}
	//joueur
	for (int i = 0; i < tailleJ;)
	{
		if (!joueurs[i])
		{
			tailleJ--;
			std::swap(joueurs[i], joueurs[tailleJ]);
			joueurs.pop_back();
		}
		else i++;
	}
//gravité
	if (temps.getElapsedTime().asSeconds() < 0.1)
	{

		for (int i = 0; i < tailleD; i++)
		{
			dynamiques[i]->forc += gravity * dynamiques[i]->masse;
		}
		for (int i = 0; i < tailleJ; i++)
		{
			joueurs[i]->forc += gravity * joueurs[i]->masse;
		}
	}


//collision

	
	for (int i = 0; i < tailleD; i++)
	{
		//dynamique - dynamique
		for (int j = i + 1; j < tailleD; j++)
		{
			dynamiques[i]->collide( *dynamiques[j]);
		}
		//dynamique - joueur
		for (int j = 0; j < tailleJ; j++)
		{
			joueurs[j]->collide( *dynamiques[i]);
		}
	}

	//statique  //n'étant pas modifier par les colision, l'ordre n'importe pas
	for (int i = 0; i < tailleS; i++)
	{
		//statique-dynamique
		for (int j = 0 ; j < tailleD; j++)
		{
			statiques[i]->collide( *dynamiques[j]);
		}
		//statique-joueur
		for (int j = 0; j < tailleJ; j++)
		{
			infoColl info = joueurs[j]->collide( *statiques[i]);
			joueurs[j]->collideJump(info);
		}
	}

//mise à jour des position en fonction du temps
	sf::Time deltaT = temps.restart();

	for (int i = 0; i < tailleD; i++)
	{
		dynamiques[i]->updatePosition(deltaT);
	}
	for (int i = 0; i < tailleJ; i++)
	{
		joueurs[i]->update(deltaT);
	}
}
void Monde::afficher(sf::RenderWindow& fenetre)
{
	for (int i = 0, taille = statiques.size(); i < taille; i++)
	{
		statiques[i]->afficher(fenetre);
	}
	for (int i = 0, taille = dynamiques.size(); i < taille; i++)
	{
		dynamiques[i]->afficher(fenetre);
	}
	for (int i = 0, taille = joueurs.size(); i < taille; i++)
	{
		joueurs[i]->afficher(fenetre);
	}
}
