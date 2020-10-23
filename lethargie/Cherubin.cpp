#include "Cherubin.h"
#include <iostream>

Cherubin::Cherubin() : Vivant()
{
	std::ifstream ifs;
	ifs.open("Ressource/MapItem/Vivant/cherubin.txt");
	LireFichier(ifs, *this);
	ifs.close();

	images_init_offset.clear();
	for (int i = 0, taille = images_offset.size(); i < taille; i++)
	{
		images_init_offset.push_back(images_offset[i]);
	}

	floating = true;
	cible = NULL;
	idleState = 0;
	timerIA = 0;
	attackState = -1;
	stabilite = 0.3;
}
void Cherubin::update(float sec)
{
	if (vivant)
	{
		IA(sec);
		animate(sec);
	}
	Vivant::update(sec);
}
void Cherubin::attack(Damage description)
{
	Vivant::attack(description);
}

void Cherubin::IA(float sec)
{
	if (cible && cible->to_destroy) cible = NULL;
	timerIA += sec;
	if (timerIA >= 0.5)
	{
		if (cible && !cible->estVivant())cible = NULL;
		if (!cible)
		{
			int taille = p_monde->joueurs.size();
			for (int i = 0; i < taille; i++)
			{
				if (p_monde->joueurs[i]->estVivant() && Float2(p_monde->joueurs[i]->getPosition() - position).norm2() < 10000.f)
				{
					cible = p_monde->joueurs[i];
					break;
				}
			}
		}

		if (cible)
		{
			if(attackState < -0.5 && lumiereMax - lumiere > 4)
			{
				attackState = 0;
			}
		}

		timerIA = 0;
	}
	if (cible)
	{
		Float2 deltaPos = position - cible->getPosition();
		float dist = deltaPos.norm2();
		if (dist > 6400)
		{
			forc -= deltaPos.setNorm(10);
		}
		else if (dist < 3600)
		{
			forc += deltaPos.setNorm(10);
		}
		else
		{
			Float2 freinage = vit * 10.f + vit.setNorm(3);
			if (freinage.norm2() > 2500)freinage.setNorm(50);
			forc -= freinage;
		}
	}
}
void Cherubin::animate(float sec)
{
	//mise à jour animation
	idleState += sec/2;
	if (idleState > 1) idleState -= 1;
	if (attackState > -0.5) attackState += sec/1.5;
	//idle
	images_offset[1] = images_init_offset[1] + Float2(0, 1) * sin(2 * PI * idleState);
	images_offset[2] = images_init_offset[2] + Float2(0, 0.5) * sin(4 * PI * idleState);
	//attack
	if(attackState > -0.5) images_offset[2] += Float2(0.5*sin(36 * PI * idleState), 0.5*sin(48 * PI * idleState));
	//lancement de sort
	if (attackState >= 1)
	{
		if (cible)
		{
			Damage cout;
			cout.lumiere = 2;
			Damage degat;
			degat.entropie = 3;

			Float2 dir = cible->getForme().formes[0].sommets[0] + cible->getPosition() - (this->getPosition() + this->images_offset[2]);
			dir.setNorm(1);
			Sort* sphere = importSort("Ressource/Sort/sphere.txt", 1, true);
			SetActorAndWorld(sphere, this);
			sphere->dommage = degat;
			sphere->setPosition(this->getPosition() + this->images_offset[2] + 2.f * dir);
			sphere->vit = this->vit + dir * 300.f;
			attack(cout);
		}
		attackState = -1;
	}
}