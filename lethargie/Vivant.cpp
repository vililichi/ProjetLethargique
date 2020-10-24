#include "Vivant.h"
#include "sort.h"
#include "FichierIO.h"

Vivant::Vivant(int _lumiereMax, int _ombreMax, float _masse, Float2 _position, Concave _forme)
	: VisibleBody(_masse, _position, _forme)
{

	images_init_offset.clear();
	for (int i = 0, taille = images_offset.size(); i < taille; i++)
	{
		images_init_offset.push_back(images_offset[i]);
	}
	lumiereMax = _lumiereMax;
	ombreMax = _ombreMax;
	for (int i = 0; i < nbrEtat; i++)
	{
		etat[i] = 0;
	}
	resplendissement = 0;
	stabilisation = 0;
	resplendissance = 1;
	stabilite = 0.5;
	lumiere = 0;
	ombre = ombreMax;
	vivant = true;
	timerMort = 0;
}
void Vivant::update(float sec)
{
	updatePosition(sec);
	if (vivant)
	{
		resplendissement += sec * resplendissance;
		stabilisation += sec * stabilite;
		if (stabilisation >= 1)
		{
			stabilisation -= 1;
			//etats stable
			if (etat[gel] > 0)
			{
				if (ombre = 0)
				{
					ombre += etat[gel];
					etat[gel] = 0;
				}
				else 
				{
					ombre += 1;
					etat[gel] -= 1;
				}

			}

			//verdict
			if (ombre > ombreMax) ombre = ombreMax;
			if (ombre <= 0)
			{
				vivant = false;
			}
			if (vivant)	ombre += 1;

		}
		if (resplendissement >= 1)
		{
			resplendissement -= 1;
			lumiere -= 1;
			//etats chaotique
			if (etat[immolation] > 0)
			{
				lumiere += 1;
				reduireOmbre(1);
				etat[immolation] -= 1;
			}

			//verdict
			if (lumiere < 0) lumiere = 0;
			if (lumiere > lumiereMax)explosionLumineuse();
		}
	}
	else animation_mort(sec);
}
void Vivant::attack(Damage description)
{
	if (vivant)
	{
		//resurection
		if (description.resurection > 0)resurection(description.resurection);
		//ombre
		ombre += description.ombre;
		//lumiere
		lumiere += description.lumiere * 2;
		//entropie
		reduireOmbre( description.entropie * 2);
		//reflexion
		if (description.reflexion > 0)
		{
			int deltaLum = lumiere;
			lumiere -= description.reflexion * 4;
			if (lumiere < 0) lumiere = 0;
			deltaLum -= lumiere;
			reduireOmbre( deltaLum);
		}
		//feux
		etat[immolation] += description.feux;
		//glace
		etat[gel] += description.glace;

		//vie max
		if (ombre > ombreMax) ombre = ombreMax;
		//valeurs négatives retirés
		if (ombre < 0) ombre = 0;
		if (lumiere < 0) lumiere = 0;
	}
}
int Vivant::getOmbre()
{
	return ombre;
}
int Vivant::getLumiere()
{
	return lumiere;
}
int Vivant::getEtat(int type)
{
	return etat[type];
}
bool Vivant::estVivant()
{
	return vivant;
}
void Vivant::resurection(int puissance )
{
	for (int i = 0; i < nbrEtat; i++)
	{
		etat[i] = 0;
	}
	if (puissance == 0)
	{
		lumiere = 0;
		ombre = ombreMax;
	}
	else
	{
		lumiere -= puissance;
		if (lumiere < 0)lumiere = 0;
		ombre += puissance;
		if (ombre > ombreMax) ombre = ombreMax;
	}
}
void Vivant::explosionLumineuse()
{
	Damage degat_lum;
	degat_lum.lumiere = lumiere / 8;
	Damage degat_jou;
	degat_jou.entropie = lumiere / 3;

	Sort* nova = importSort("Ressource/Sort/nova.txt", 0.3, true, true, true, false);
	SetActorAndWorld(nova, this, true);

	nova->setPosition(getPosition());
	float size (RigidBody::getApproxTaille() / 3);
	nova->resize(Float2(size,size));
	nova->dommage = degat_lum;
	attack(degat_jou);
	lumiere = 0;
}
void Vivant::reduireOmbre(int valeur)
{
	while (etat[gel] > 0 && valeur > 0)
	{
		valeur -= 2;
		etat[gel] -= 1;
	}

	if (valeur > 0)
	{
		ombre -= valeur;
	}
}
float Vivant::coefVitesse()
{
	float retour = 1;
	if (etat[gel] > 1) retour *= 0.5;


	return retour;
}
void Vivant::animation_mort(float sec)
{
	if (vivant) return;
	timerMort += sec / 5;
	floating = false;
	if (timerMort >= 1)
	{
		to_destroy = true;
		return;
	}
	for (int i = 0; i < images.size(); i++)
	{
		sf::Color couleur = images[i].getColor();
		couleur.a = 256 - timerMort * 256;
		images[i].setColor(couleur);
	}
	
}