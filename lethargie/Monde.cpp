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
Sort* Monde::addSort(Sort new_sort)
{
	Sort* p_new_sort = new Sort(new_sort);
	p_new_sort->setMonde(this);
	sorts.push_back(p_new_sort);
	return p_new_sort;
}
Sort* Monde::addSort(Sort* p_new_sort)
{
	p_new_sort->setMonde(this);
	sorts.push_back(p_new_sort);
	return p_new_sort;
}
void Monde::update()
{
//nettoyage des pointeur null et calcul tailles

	//tailles initiale
	int tailleS = statiques.size();
	int tailleD = dynamiques.size();
	int tailleJ = joueurs.size();
	int tailleSS = sorts.size();

	//statique
	for (int i = 0; i < tailleS;)
	{
		if (statiques[i]->to_destroy)
		{
			delete statiques[i];
			tailleS--;
			std::swap(statiques[i], statiques[tailleS]);
			statiques.pop_back();
		}
		else i++;
	}
	//dynamique
	for (int i = 0; i < tailleD;)
	{
		if (dynamiques[i]->to_destroy)
		{
			delete dynamiques[i];
			tailleD--;
			std::swap(dynamiques[i], dynamiques[tailleD]);
			dynamiques.pop_back();
		}
		else i++;
	}
	//joueur
	for (int i = 0; i < tailleJ;)
	{

		if (joueurs[i]->to_destroy)
		{
			delete joueurs[i];
			tailleJ--;
			std::swap(joueurs[i], joueurs[tailleJ]);
			joueurs.pop_back();
		}
		else i++;
	}
	//sort
	for (int i = 0; i < tailleSS;)
	{
	
		if (sorts[i]->to_destroy)
		{
			delete sorts[i];
			tailleSS--;
			std::swap(sorts[i], sorts[tailleSS]);
			sorts.pop_back();
		}
		else i++;
	}
//gravité

	for (int i = 0; i < tailleD; i++)
	{
		dynamiques[i]->forc += gravity * dynamiques[i]->masse;
	}
	for (int i = 0; i < tailleJ; i++)
	{
		joueurs[i]->forc += gravity * joueurs[i]->masse;
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

	//statique  //n'étant pas modifier par les collision, l'ordre n'importe pas
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
			infoColl info = joueurs[j]->collide(*statiques[i]);
			joueurs[j]->collideJump(info);
		}
	}

	//sort
	for (int i = 0; i < tailleSS; i++)
	{
		//sort mur(static
		for (int j = 0; j < tailleS; j++)
		{
			sorts[i]->collideWall(*statiques[j]);
		}
		//statique-joueur
		for (int j = 0; j < tailleJ; j++)
		{
			sorts[i]->collideVivant(*joueurs[j]);
		}
	}

//mise à jour des position et états en fonction du temps
	float deltaT = temps.restart().asSeconds();
	if (deltaT > 0.02)
	{
		deltaT = 0;
	}

	for (int i = 0; i < tailleD; i++)
	{
		dynamiques[i]->updatePosition(deltaT);
	}
	for (int i = 0; i < tailleJ; i++)
	{
		joueurs[i]->update(deltaT);
	}
	for (int i = 0; i < tailleSS; i++)
	{
		sorts[i]->update(deltaT);
	}
}
void Monde::afficher(sf::RenderWindow& fenetre)
{
	for (int i = 0, taille = statiques.size(); i < taille; i++)
	{
		statiques[i]->afficher(fenetre);
	}
	for (int i = 0, taille = sorts.size(); i < taille; i++)
	{
		sorts[i]->afficher(fenetre);
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
