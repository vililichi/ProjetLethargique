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
	int ombre;
	int lumiere;
	bool vivant;
	int etat[nbrEtat];
	float stabilisation;
	float resplendissement;
	std::vector<Float2> images_init_offset;
public:

	int ombreMax;
	int lumiereMax;
	float stabilite;
	float resplendissance;


	Vivant(int _lumiereMax = 10, int _ombreMax = 10,float _masse = 1, Float2 _position = Float2(0,0), Concave _forme = Concave() );
	void update(float sec);
	void attack(Damage description);
	int getOmbre();
	int getLumiere();
	int getEtat(int type);
	bool estVivant();
	void resurection(int puissance = 0);
	void explosionLumineuse();
	void reduireOmbre(int valeur);
	float coefVitesse();
};
