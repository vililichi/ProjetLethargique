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
Vivant* Monde::addVivant(Vivant* new_vivant)
{
	vivants.push_back(new_vivant);
	new_vivant->setMonde(this);
	return new_vivant;
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
	int tailleV = vivants.size();
	int tailleSS = sorts.size();

	std::vector<RigidBody*> trashRB;
	std::vector<Joueur*> trashJ;
	std::vector<Vivant*> trashV;
	std::vector<Sort*> trashS;



	//statique
	for (int i = 0; i < tailleS;)
	{
		if (statiques[i]->to_destroy)
		{
			trashRB.push_back( statiques[i]);
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
			trashRB.push_back(dynamiques[i]);
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
			trashJ.push_back(joueurs[i]);
			tailleJ--;
			std::swap(joueurs[i], joueurs[tailleJ]);
			joueurs.pop_back();
		}
		else i++;
	}
	//vivants
	for (int i = 0; i < tailleV;)
	{
		if (vivants[i]->to_destroy)
		{
			trashV.push_back( vivants[i]);
			tailleV--;
			std::swap(vivants[i], vivants[tailleV]);
			vivants.pop_back();
		}
		else i++;
	}
	//sort
	for (int i = 0; i < tailleSS;)
	{
	
		if (sorts[i]->to_destroy)
		{
			trashS.push_back( sorts[i]);
			tailleSS--;
			std::swap(sorts[i], sorts[tailleSS]);
			sorts.pop_back();
		}
		else i++;
	}
//gravité

	for (int i = 0; i < tailleD; i++)
	{
		dynamiques[i]->addGravity(gravity);
	}
	for (int i = 0; i < tailleJ; i++)
	{
		joueurs[i]->addGravity(gravity);
	}
	for (int i = 0; i < tailleSS; i++)
	{
		sorts[i]->addGravity(gravity);
	}
	for (int i = 0; i < tailleV; i++)
	{
		vivants[i]->addGravity(gravity);
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
		//dynamique - vivant
		for (int j = 0; j < tailleV; j++)
		{
			vivants[j]->collide(*dynamiques[i]);
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
		//statique-vivant
		for (int j = 0; j < tailleV; j++)
		{
			infoColl info = vivants[j]->collide(*statiques[i]);
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
		//sort-joueur
		for (int j = 0; j < tailleJ; j++)
		{
			sorts[i]->collideVivant(*joueurs[j]);
		}
		//sort-vivant
		for (int j = 0; j < tailleV; j++)
		{
			sorts[i]->collideVivant(*vivants[j]);
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
	for (int i = 0; i < tailleV; i++)
	{
		vivants[i]->update(deltaT);
	}
	for (int i = 0; i < tailleSS; i++)
	{
		sorts[i]->update(deltaT);
	}
	//destruction
	for (int i = 0; i < trashRB.size(); i++) delete trashRB[i];
	for (int i = 0; i < trashJ.size(); i++) delete trashJ[i];
	for (int i = 0; i < trashV.size(); i++) delete trashV[i];
	for (int i = 0; i < trashS.size(); i++) delete trashS[i];
	trashRB.clear();
	trashJ.clear();
	trashS.clear();
	trashV.clear();
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
	for (int i = 0, taille = vivants.size(); i < taille; i++)
	{
		vivants[i]->afficher(fenetre);
	}
	for (int i = 0, taille = joueurs.size(); i < taille; i++)
	{
		joueurs[i]->afficher(fenetre);
	}
}
