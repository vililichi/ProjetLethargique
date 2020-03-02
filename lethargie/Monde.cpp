#include "Monde.h"

Monde::Monde()
{
	clear();
	temps.restart();
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
	statiques.clear();
	dynamiques.clear();
}
corps_visible* Monde::addStatique(corps_visible new_corps)
{
	corps_visible* p_new_corps = new corps_visible(new_corps);
	p_new_corps->is_Dynamic = false;
	statiques.push_back(p_new_corps);
	return p_new_corps;
}
corps_visible* Monde::addDynamique(corps_visible new_corps)
{
	corps_visible* p_new_corps = new corps_visible(new_corps);
	p_new_corps->is_Dynamic = true;
	dynamiques.push_back(p_new_corps);
	return p_new_corps;
}
void Monde::update()
{
//nettoyage des pointeur null et calcul tailles

	//tailles initiale
	int tailleS = statiques.size();
	int tailleD = dynamiques.size();

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

//collision

	//dynamique - dynamique
	for (int i = 0; i < tailleD; i++)
	{
		for (int j = i + 1; j < tailleD; j++)
		{
			*dynamiques[i] + *dynamiques[j];
		}
		for (int j = 0; j < tailleS; j++)
		{
			*dynamiques[i] + *dynamiques[j];
		}
	}

	//statique  //n'étant pas modifier par les colision, l'ordre n'importe pas
	for (int i = 0; i < tailleS; i++)
	{
		//statique-dynamique
		for (int j = 0 ; j < tailleD; j++)
		{
			*statiques[i] + *dynamiques[j];
		}
		//statique-autre
	}

//mise à jour des position en fonction du temps
	sf::Time deltaT = temps.restart();

	for (int i = 0; i < tailleD; i++)
	{
		dynamiques[i]->updatePosition(deltaT);
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
}
