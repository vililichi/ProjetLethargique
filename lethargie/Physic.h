#pragma once

#include"Math_addOn.h"
#include<SFML/System/Clock.hpp>
#include<SFML/Graphics.hpp>

class Monde;
class RigidBody
{
protected:
	bool modif;
	Concave forme;
	Concave worldForme;
	//estimation de la taille
	float approxTaille;
	Float2 position;
	Float2 old_position;
	Float2 size;

	void calculApproxTaille();

public:
	bool to_destroy;
	bool floating;
	float bounce, friction, masse;

	Float2 vit;
	Float2 acc;
	Float2 forc;
	Monde* p_monde;

	bool is_Dynamic;
	RigidBody();
	RigidBody(float _masse, Float2 _position, Concave _forme, bool dynamic = 1);
	void setForme(Concave new_forme);
	Concave getForme() const;
	Concave getWorldForme();
	virtual void setPosition(Float2 new_position);
	virtual void rotate(float angle);
	Float2 getPosition() const;
	float getApproxTaille() const;
	virtual void updatePosition(float sec);
	virtual void addGravity(Float2 gravity);
	infoColl testCollision (RigidBody& c);
	infoColl collide (RigidBody& c);
	virtual void resize(Float2 multiplicateur);
	Float2 getSize();
	void setMonde(Monde* p_newMonde);
};

struct collisionSolution
{
	Float2 normal = Float2(0, 1);
	float factor = NAN;
};

collisionSolution solveCollision(Float2 pactu, Float2 pancien, Convexe& forme, Float2 posConvexe, Float2 posConvexeAncien);

class VisibleBody : public RigidBody
{
protected:
	bool modif_images;
	std::vector<sf::Sprite> images;
	
public:
	int layer;
	std::vector<Float2> images_offset;

	VisibleBody();
	VisibleBody(float _masse, Float2 _position, Concave _forme, bool dynamic = 1);

	void clear_images();
	void add_images(sf::Sprite image,Float2 offset = Float2(0,0));

	void afficher( sf::RenderWindow& fenetre);

	virtual void setPosition(Float2 new_position);
	virtual void rotate(float angle);
	virtual void updatePosition(float sec);
	virtual void resize(Float2 multiplicateur);

	friend int LireFichier(std::ifstream& fichier, VisibleBody& contenant); //définie dans FichierIO.cpp
	friend int EcrireFichier(std::ofstream& fichier, VisibleBody& objet); //définie dans FichierIO.cpp
};