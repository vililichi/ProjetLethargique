#include "arme.h"

#pragma region base
//competence
Competence::Competence()
{
	animDuree = 1;
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
//arme
Arme::Arme()
{
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
	}
	else if (noCompetence == 2)
	{
		if (competence2 != NULL) delete competence2;
		competence2 = competence;
	}
}

void Arme::update(float sec)
{
	if (competence1 != NULL) competence1->update(sec);
	if (competence2 != NULL) competence2->update(sec);
}




#pragma endregion

#pragma region infusion

#pragma endregion

#pragma redion aiguille

#pragma endregion