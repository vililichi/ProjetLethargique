#include "Math_addOn.h"
#pragma region Float2
Float2::Float2()
{
	x = 0;
	y = 0;
}
Float2::Float2(float X, float Y)
{
	x = X;
	y = Y;
}
Float2::Float2(sf::Vector2f Vector)
{
	x = Vector.x;
	y = Vector.y;

}

float Float2::norm()
{
	return sqrtf(norm2());
}
float Float2::norm2()
{
	return x * x + y * y;
}

float Float2::angle()
{
	float angle;
	float div = y / x;
	if (isnan(div))
	{
		if (y > 0) angle = PI / 2;
		else angle = 3 * PI / 2;
	}
	else
	{
		angle = atanf(y / x);
		if (x < 0)angle += PI;
	}

	return angle;
}

Float2& Float2::setNorm(float norm)
{
	float ratio = norm / Float2::norm();
	x *= ratio;
	y *= ratio;
	return *this;
}

Float2& Float2::setAngle(float angle)
{
	float norm = Float2::norm();
	y = norm * sinf(angle);
	x = norm * cosf(angle);
	return *this;
}

float Float2::operator * (Float2& u)
{
	return u.x * x + u.y * y;
}

Float2 Float2::operator / (Float2& u)
{
	return ((Float2::operator* (u)) / u.norm2()) * u;
}

std::ostream& operator<<(std::ostream& os, const Float2& u)
{
	os << u.x << "," << u.y;
	return os;
}

Float2& operator>>(std::istream& is, Float2& u)
{
	char trash;
	is >> u.x;
	is >> trash;
	is >> u.y;
	return u;
}

Float2 Float2::operator = (Float2 u)
{
	x = u.x;
	y = u.y;
	return *this ;
}
#pragma endregion
#pragma region Droite
Droite::Droite()
{
	A = 0;
	B = 0;
}

Droite::Droite(Float2 u, Float2 v)
{
	if (u == v)
	{
		//infinité de solution, choix d'une solution par défaut
		if (u.x == 0)
		{
			A = 0;
			B = u.y;
		}
		else
		{
			A = u.y / u.x;
			B = 0;
		}
	}
	else if (u.x == v.x)
	{
		//Aucune fonction possible droite vertical
		A = NAN;
		B = u.x;
	}
	else
	{
		//Une seule solution possible
		A = (u.y - v.y) / (u.x - v.x);
		B = u.y - (A * u.x);
	}
}

bool Droite::operator == (Float2& p)
{
	if (isnan(A))
	{
		return B == p.x;
	}
	else
	{
		return p.y == ((p.x * A) + B);
	}
}
bool Droite::operator < (Float2& p)
{
	if (isnan(A))
	{
		return B < p.x;
	}
	else
	{
		return ((p.x * A) + B) < p.y;
	}

}
bool Droite::operator > (Float2& p)
{
	if (isnan(A))
	{
		return B > p.x;
	}
	else
	{
		return ((p.x * A) + B) > p.y;
	}

}
bool Droite::operator <= (Float2& p)
{
	if (isnan(A))
	{
		return B >= p.x;
	}
	else
	{
		return ((p.x * A) + B) <= p.y;
	}

}
bool Droite::operator >= (Float2& p)
{
	if (isnan(A))
	{
		return B >= p.x;
	}
	else
	{
		return ((p.x * A) + B) >= p.y;
	}

}
bool operator == (Float2& p, Droite& d) { return d == p; }
bool operator < (Float2& p, Droite& d) { return d > p; }
bool operator > (Float2& p, Droite& d) { return d < p; }
bool operator <= (Float2& p, Droite& d) { return d >= p; }
bool operator >= (Float2& p, Droite& d) { return d <= p; }
bool Droite::operator != (Float2& p)
{
	if (isnan(A))
	{
		return B != p.x;
	}
	else
	{
		return p.y != ((p.x * A) + B);
	}
}
bool operator != (Float2& p, Droite& d) { return d != p; }


//point de rencontre
Float2 Droite::operator * (Droite& d)
{
	Float2 p;
	if (isnan(A) && isnan(d.A))
	{
		if (B == d.B)
		{
			p.x = B;
			p.y = NAN;
		}
		else
		{
			p.x = NAN;
			p.y = NAN;
		}
	}
	else if (isnan(A))
	{
		p.x = B;
		p.y = d.A * B;

	}
	else if (isnan(d.A))
	{
		p.x = d.B;
		p.y = A * d.B;

	}
	else
	{
		p.x = (d.B - B) / (A - d.A);
		p.y = p.x * A;
	}
	return p;
}

std::ostream& operator<<(std::ostream& os, const Droite& u)
{
	if (isnan(u.A)) os << "X = " << u.B;
	else os << "Y = " << u.A << "X + " << u.B;
	return os;
}
#pragma endregion
#pragma region Convexe
Convexe::Convexe()
{
	sommets.clear();
}

unsigned int Convexe::size()
{
	return sommets.size();
}

infoColl Convexe::operator * (Convexe shape)
{
	bool collision = false;
	unsigned short tailleA;
	unsigned short tailleB;

	//obtention taille
	tailleA = size();
	tailleB = shape.size();

	//calcul centre
	Float2 CentreA(0,0);
	Float2 CentreB(0,0);


	for (int i = 0; i < tailleA; i++)
	{
		CentreA += sommets[i];
	}
	for (int i = 0; i < tailleB; i++)
	{
		CentreB += shape.sommets[i];
	}


	CentreA /= (float)tailleA;
	CentreB /= (float)tailleB;

	infoColl information;
	information.firstElem[0] = false;

	//test colision A->B
	unsigned short i = 0;
	while ( i < tailleA)
	{
		collision = true;
		for(unsigned short j = 0; j < tailleB; j++)
		{
			Float2 u;
			if (j == 0) u = shape.sommets[tailleB - 1];
			else u = shape.sommets[j - 1];
			Droite d(u,shape.sommets[j]);
			bool sens = CentreB < d;
			bool positionR = sommets[i] <= d;
			if (positionR != sens) collision = false;
		}
		if (collision)
		{
			information.firstElem[information.taille] = true;
			information.point[information.taille] = sommets[i];
			information.taille++;
		}
		i++;
	}
	
		//test colison B->A
		i = 0;
		while (i < tailleB)
		{
			collision = true;
			for (unsigned short j = 0; j < tailleA; j++)
			{
				Float2 u;
				if (j == 0) u = sommets[tailleA - 1];
				else u = sommets[j - 1];
				Droite d(u, sommets[j]);
				bool sens = CentreA < d;
				bool positionR = shape.sommets[i] <= d;
				if (positionR != sens) collision = false;
			}
			if (collision)
			{
				information.firstElem[information.taille] = false;
				information.point[information.taille] = shape.sommets[i];
				information.taille++;
			}
			i++;
		}
		
		
	return information;
}
#pragma endregion
#pragma region Concave
Concave::Concave()
{
	formes.clear();
}
unsigned int Concave::size()
{
	return formes.size();
}
//collision
infoColl Concave::operator * (Concave shape)
{
	short taille = size();
	infoColl retour;
	for(short i = 0; i < taille;i++)
	{
		infoColl info = formes[i] * shape;
		int taillej = info.taille;
		for (int j = 0; j < taillej; j++)
		{
			if (!info.firstElem[j]) info.index[j] = i;
			retour.index[retour.taille] = info.index[j];
			retour.firstElem[retour.taille] = info.firstElem[j];
			retour.point[retour.taille] = info.point[j];
			retour.taille++;
		}
	}
	return retour;
}
infoColl Concave::operator * (Convexe shape) 
{
	short i = 0;
	short taille = size();
	infoColl retour;
	while (i < taille)
	{
		infoColl info = formes[i] * shape;
		int taillej = info.taille;
		for(int j = 0; j<taillej; j++)
		{

			if(retour.firstElem[retour.taille] = info.firstElem[j])
				retour.index[retour.taille] = 0;
			else retour.index[retour.taille] = i;
			retour.point[retour.taille] = info.point[j];
			retour.taille++;
		}
		i++;
	}
	return retour;
}
infoColl operator * (Convexe shape, Concave shape2)
{
	infoColl info = shape2 * shape;
	int taille = info.taille;
	for (int i = 0; i< taille; i++)
	{
		if (info.firstElem[i])info.firstElem[i] = false;
 		else info.firstElem[i] = true;
	}
	return info;
}
#pragma endregion
