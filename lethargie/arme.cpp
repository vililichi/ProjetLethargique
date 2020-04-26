#include "arme.h"
#include "Joueur.h"

#pragma region base
//competence
const int bras1 = 0;
const int bras2 = 5;
Competence::Competence()
{
	animDuree = 0.5;
	animEtat = 0;
	animActif = false;
	acteur = NULL;
	position = Float2(0, 0);
	objectif = Float2(0, 0);
}
void Competence::update( float sec)
{
	if (animActif)
	{
		animEtat += sec / animDuree;
		if (animEtat > 1)
		{
			animActif = false;
			animEtat = 0;
		}
		anim(sec);
	}
}
void Competence::activate()
{
	animActif = true;
	position = acteur->getPosition() + acteur->images_offset[bras2];
	objectif = acteur->actionFutur.pointeur;
}
//arme
Arme::Arme()
{
	possesseur = NULL;
	competence1 = NULL;
	competence2 = NULL;
}
Arme::~Arme()
{
	if (competence1 != NULL) delete competence1;
	if (competence2 != NULL) delete competence2;
}

void Arme::setCompetence(Competence* competence, short noCompetence)
{
	if (noCompetence == 1)
	{
		if (competence1 != NULL) delete competence1;
		competence1 = competence;
		if(possesseur) competence1->acteur = possesseur;
	}
	else if (noCompetence == 2)
	{
		if (competence2 != NULL) delete competence2;
		competence2 = competence;
		if (possesseur) competence2->acteur = possesseur;
	}
}

void Arme::update(float sec)
{
	if (!competence1->animActif && !competence2->animActif)
	{
		if (possesseur->actionFutur.action1)
		{
			competence1->activate();
		}
		else if (possesseur->actionFutur.action2)
		{
			competence2->activate();
		}
	}
	if (competence1 != NULL) competence1->update(sec);
	if (competence2 != NULL) competence2->update(sec);
}

void Arme::bound(Joueur* _possesseur)
{
	possesseur = _possesseur;
	if(competence1) competence1->acteur = possesseur;
	if(competence2) competence2->acteur = possesseur;
}


#pragma endregion

#pragma region infusion

void InfusionSphere::anim(float sec)
{
	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	acteur->images_offset[bras2] += 14.f*dir*sin(animEtat*PI);
}

void InfusionDart::anim(float sec)
{

	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	if (animEtat < 0.2)
	{
		dir.setAngle(dir.angle() - PI/6);
		acteur->images_offset[bras2] += 14.f * dir * (float)sin(animEtat * (float)PI / 0.4f);
	}
	else if (animEtat < 0.8)
	{
		dir.setAngle(dir.angle() - PI/6 + PI/3 * sin((animEtat-0.2f)* (float)PI/1.2f));
		acteur->images_offset[bras2] += 14.f * dir;
	}
	else
	{
		dir.setAngle(dir.angle() + PI / 6);
		acteur->images_offset[bras2] += 14.f * dir * (float)sin(-1.f*(animEtat-0.8f) * (float)PI / 0.4f + (float)PI/2.f);
	}
	
}

void InfusionNova::anim(float sec)
{
	Float2 offset = Float2(0, -14) * sin(animEtat * 2 * PI) * sin((-1* animEtat / 2 * PI) + PI/2);
	acteur->images_offset[bras1] += offset;
	acteur->images_offset[bras2] += offset;
}

void InfusionExplosion::anim(float sec)
{
	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	acteur->images_offset[bras2] += 14.f * dir * sin(animEtat * PI);
}

#pragma endregion

#pragma region aiguille
void AiguillePercante::anim(float sec) 
{
	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	acteur->images_offset[bras2] += 5.f * dir * sin(animEtat * PI);
}

void AiguilleTaillante::anim(float sec)
{
	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	acteur->images_offset[bras2] += 5.f * dir * sin(animEtat * PI);
}

void AiguilleCirculaire::anim(float sec)
{
	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	acteur->images_offset[bras2] += 5.f * dir * sin(animEtat * PI);
}

void AiguilleDistante::anim(float sec)
{
	Float2 dir = ((Float2)(objectif - position)).setNorm(1);
	acteur->images_offset[bras2] += 5.f * dir * sin(animEtat * PI);
}

#pragma endregion