#pragma once
#include "Physic.h"

const int nbrEtat = 2;
const int immolation = 0, gel = 1;


struct Damage
{
	int resurection = 0;
	int ombre = 0; //ajoute 2 ombre
	int lumiere = 0; //ajoute 2 lumiere
	int entropie = 0; //retire 2 ombre
	int reflexion = 0; // retire 4 lumiere et autant d'ombre
	int feux = 0; //ajoute 1 immolation
	int glace = 0; // ajoute 1 gel
};
class Competence;
class Vivant :public VisibleBody
{
	friend Competence;
protected:
	int ombre;				//mort si tombe � 0
	int lumiere;			//explose si tombe � lumiereMax
	bool vivant;			//indique si l'entit� est vivante
	int etat[nbrEtat];		//En cours de travail, gestion des diff�rents alt�rations
	float stabilisation;	//recharge et update l'ombre � 1
	float resplendissement; //recharge et update la lumi�re � 1
	std::vector<Float2> images_init_offset;
	float timerMort;
public:

	int ombreMax;			//ombre maximale pouvant �tre atteinte
	int lumiereMax;			//seuil d'explosion
	float stabilite;		//stabilisation � chaque 1/stabilite secondes; d�fault 0.5
	float resplendissance;	//resplandissement � chaque 1/resplandissance secondes; d�fault 1


	Vivant(int _lumiereMax = 10, int _ombreMax = 10,float _masse = 1, Float2 _position = Float2(0,0), Concave _forme = Concave() );
	virtual void update(float sec);
	virtual void attack(Damage description);
	virtual void animation_mort(float sec);
	int getOmbre();
	int getLumiere();
	int getEtat(int type);
	bool estVivant();
	void resurection(int puissance = 0);
	void explosionLumineuse();
	void reduireOmbre(int valeur);
	float coefVitesse();
};
