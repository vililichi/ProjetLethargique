#pragma once
#include <SFML/System/Vector2.hpp>
#include <math.h>
#include <iostream>
#include <vector>

#ifndef PI
#define PI (float)3.14159265359
#endif



class Float2 : public sf::Vector2f
{
public:
	Float2();
	Float2(float X, float Y);
	Float2(sf::Vector2f Vector);

	//retourne la norme du vecteur
	float norm();
	float norm2();

	//retourne l'angle du vecteur
	float angle();

	//change la norme du vecteur sans changer son angle
	Float2& setNorm(float norm);

	//change l'angle du vecteur sans changer sa norme
	Float2& setAngle(float angle);

	//produit scalaire
	float operator * (Float2& u);

	//projection orthogonale
	Float2 operator / (Float2& u);

	//stream
	friend std::ostream& operator<<(std::ostream& os, const Float2& u);

	friend Float2& operator>>(std::istream& is, Float2& u);

	//=
	Float2 operator = (Float2 u);
};

class Droite
{
public:
	float A;
	float B;
	Droite();
	Droite(Float2, Float2);

	bool operator == (Float2& p);
	bool operator < (Float2& p);
	bool operator > (Float2& p);
	bool operator <= (Float2& p);
	bool operator >= (Float2& p);
	friend bool operator == (Float2& p, Droite& d);
	friend bool operator < (Float2& p, Droite& d);
	friend bool operator > (Float2& p, Droite& d);
	friend bool operator <= (Float2& p, Droite& d);
	friend bool operator >= (Float2& p, Droite& d);
	bool operator != (Float2& p);
	friend bool operator != (Float2& p, Droite& d);

	//point de rencontre
	Float2 operator * (Droite& d);

	friend std::ostream& operator<<(std::ostream& os, const Droite& u);

};

struct infoColl
{
	Float2 point[100] = {Float2()};
	bool firstElem[100] = {0};
	int index[100] = {0};
	int taille = 0;
};

class Convexe
{
public:
	std::vector<Float2> sommets;

	Convexe();
	unsigned int size();
	//collision
	infoColl operator * (Convexe shape);

	
	
};

class Concave
{
public:
	std::vector<Convexe> formes;

	Concave();
	unsigned int size();
	//collision
	infoColl operator * (Concave shape);
	infoColl operator * (Convexe shape);
	friend infoColl operator * (Convexe shape, Concave shape2);
};
