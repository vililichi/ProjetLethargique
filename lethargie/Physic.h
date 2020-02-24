#pragma once

#include"Math_addOn.h"
#include<SFML/System/Clock.hpp>
#include<SFML/Graphics.hpp>

class corps
{
protected:
	bool modif;
	Concave forme;
	Concave worldForme;
	//estimation de la taille
	float approxTaille;
	Float2 position;
	Float2 old_position;

public:
	float bounce, friction, masse;

	Float2 vit;
	Float2 acc;
	Float2 forc;

	bool is_Dynamic;
	corps();
	corps(float _masse, Float2 _position, Concave _forme, bool dynamic = 1);
	void setForme(Concave new_forme);
	Concave getForme() const;
	Concave getWorldForme();
	void setPosition(Float2 new_position);
	Float2 getPosition() const;
	float getApproxTaille() const;
	void updatePosition(sf::Time deltaT);
	infoColl operator * (corps& c);
	infoColl operator + (corps& c);
};

struct collisionSolution
{
	Float2 normal = Float2(0, 1);
	float factor = -2;
};

collisionSolution solveCollision(Float2 pactu, Float2 pancien, Convexe& forme, Float2 posConvexe, Float2 posConvexeAncien);

class corps_visible : public corps
{
protected:
	bool modif_images;
	std::vector<sf::Sprite> images;
	std::vector<Float2> images_offet;
	
public:
	int layer;

	corps_visible();
	corps_visible(float _masse, Float2 _position, Concave _forme, bool dynamic = 1);

	void clear_images();
	void add_images(sf::Sprite image,Float2 offset = Float2(0,0));

	void afficher( sf::RenderWindow& fenetre);

	virtual void setPosition(Float2 new_position);
	virtual void setOrientation(float new_orientation);
	virtual void updatePosition(sf::Time deltaT);
};