#include "Physic.h"
#pragma region corps
Float2 doBounce(Float2& v, Float2 normal, float bouncing)
{
	float delta = v.angle() - normal.angle();
	if (isnan(delta)) return v;
	if (delta < -PI / 2) delta += 2 * PI;
	else if (delta > 3 * PI / 2) delta -= 2 * PI;

	if (delta > PI / 2) {
		v -= (v / normal) * (1 + bouncing);
	}
	return v;
}
RigidBody::RigidBody()
{
	size = Float2(1, 1);
	forme = Concave();
	worldForme = Concave();
	approxTaille = 0;
	modif = false;
	position.x = 0;
	position.y = 0;
	old_position = Float2(0,0);
	masse = 0;
	is_Dynamic = 0;
	bounce = 0.5;
	friction = 0.5;
	p_monde = NULL;

}
RigidBody::RigidBody(float _masse, Float2 _position, Concave _forme, bool dynamic)
{
	size = Float2(1, 1);
	forme = Concave();
	worldForme = Concave();
	old_position = _position;
	setPosition(_position);
	setForme(_forme);
	masse = _masse;
	modif = true;
	is_Dynamic = dynamic;
	bounce = 0.5;
	friction = 0.5;
	p_monde = NULL;

}
void RigidBody::setForme(Concave new_forme)
{
	forme = new_forme;
	calculApproxTaille();
	modif = true;
}
Concave RigidBody::getForme() const
{
	return forme;
}
Concave RigidBody::getWorldForme()
{
	if (modif)
	{
		worldForme = forme;
		short taille = forme.size();
		for (short i = 0; i < taille; i++)
		{
			short taille2 = worldForme.formes[i].size();
			for (short j = 0; j < taille2; j++)
			{
				worldForme.formes[i].sommets[j] += position;
			}
		}
		modif = false;
	}
	
	return worldForme;
}
void RigidBody::setPosition(Float2 new_position) 
{
	position = new_position;
	if (!is_Dynamic)old_position = new_position;
	modif = true;
}
Float2 RigidBody::getPosition() const 
{
	return position;
}
float RigidBody::getApproxTaille() const
{
	return approxTaille;

}
void RigidBody::updatePosition(sf::Time deltaT)
{
	old_position = position;
	float sec = deltaT.asSeconds();
		modif = true;
		acc += forc  / masse;
		position += (acc * sec * sec * 0.5f) + vit * sec;
		vit += acc * sec;

		acc = Float2(0, 0);
		forc = Float2(0, 0);


}
infoColl RigidBody::testCollision (RigidBody& c)
{
	infoColl info;
	bool col = true;
	Float2 dir = position - c.getPosition();
	float dist = dir.norm2();
	float tailleSqrt = approxTaille + c.getApproxTaille();
	tailleSqrt *= tailleSqrt;
	if (dist > tailleSqrt) {
		col = false;
	}
	else
	{
		info = getWorldForme() * c.getWorldForme();
	}

	return info;
}
infoColl RigidBody::collide (RigidBody& c)
{
	infoColl collision = testCollision(c);
	if (collision.taille == 0)return collision;

	RigidBody* firstF = this;
	RigidBody* secondF = &c;

	collisionSolution fact;
	for (int i = 0, tailleCol = collision.taille; i < tailleCol; i++)
	{
		RigidBody* first;
		RigidBody* second;
		bool stop = false;

		if (collision.firstElem[i])
		{
			stop = true;
			first = this;
			second = &c;

		}
		else
		{
			first = &c;
			second = this;
		}

		const Float2 pactu = first->position;
		const Float2 pancien = first->old_position;

		const Float2 pactu2 = second->position;
		const Float2 pancien2 = second->old_position;

		Convexe formeCol = second->getWorldForme().formes[collision.index[i]];


		int taille = formeCol.size();

		collisionSolution factT = solveCollision(collision.point[i], collision.point[i] - pactu + pancien, formeCol, pactu2, pancien2);
			
		if (factT.factor > fact.factor)
		{
			fact = factT;
			firstF = first;
			secondF = second;
		}
	}
		if (firstF->is_Dynamic && secondF->is_Dynamic)
		{
			//position
			setPosition(position + fact.factor * (old_position - position));
			c.setPosition(c.position + fact.factor * (c.old_position - c.position));


			Float2 normal = fact.normal;

			float smasse = masse + c.masse;

			Float2 v1 = firstF->vit / normal;
			Float2 v2 = secondF->vit / normal;

			firstF->vit -= v1;
			firstF->vit += v1 * (firstF->masse - secondF->masse) / smasse + v2 * 2.f * secondF->masse / smasse;

			secondF->vit -= v2;
			secondF->vit += v2 * (secondF->masse - firstF->masse) / smasse + v2 * 2.f * firstF->masse / smasse;



		}
		else if (firstF->is_Dynamic)
		{
			//normal
			Float2 normal = fact.normal;
			//position
			
			if (fact.factor <= -1)fact.factor = 1.5;
			firstF->setPosition(firstF->position + fact.factor * ((Float2)(firstF->old_position - firstF->position)/normal));
			
			//normal

			doBounce(firstF->forc, normal, 1);

			//vitesse
			doBounce(firstF->vit, normal, bounce * c.bounce);

			//ground
			collision.grounded = true;
			collision.groundDir = normal;
		}
		else if (secondF->is_Dynamic)
		{
			//normal
			Float2 normal = -fact.normal;

			//position
			
			if (fact.factor <= -1)fact.factor = 1.5;
			secondF->setPosition(secondF->position + fact.factor * ((Float2)(firstF->old_position - firstF->position) / normal));
			
		
			//force
			doBounce(secondF->forc, normal, 1);

			//vitesse
			doBounce(secondF->vit, normal, bounce * c.bounce);

			//ground
			collision.grounded = true;
			collision.groundDir = normal;
		}
	return collision;
}
void RigidBody::resize(Float2 multiplicateur)
{
	for (int i = 0, taillei = forme.size(); i < taillei; i++)
	{
		for (int j = 0, taillej = forme.formes[i].size(); j < taillej; j++)
		{
			forme.formes[i].sommets[j].x *= multiplicateur.x / size.x;
			forme.formes[i].sommets[j].y *= multiplicateur.y / size.y;
		}
	}
	calculApproxTaille();
	modif = true;
	size = multiplicateur;
}
Float2 RigidBody::getSize()
{
	return size;
}
void RigidBody::calculApproxTaille()
{
	float max = 0;
	for (short i = 0, taille = forme.size(); i < taille; i++)
	{
		short taille2 = forme.formes[i].size();
		for (short j = 0; j < taille2; j++)
		{
			float x = forme.formes[i].sommets[j].x;
			float y = forme.formes[i].sommets[j].y;
			float tailleJ = x * x + y * y;
			if (tailleJ > max)max = tailleJ;
		}
	}
	approxTaille = sqrtf(max);
}
void RigidBody::setMonde(Monde* p_newMonde)
{
	p_monde = p_newMonde;
}

collisionSolution solveCollision(Float2 pactu, Float2 pancien, Convexe& forme, Float2 posConvexe, Float2 posConvexeAncien)
{
	
	collisionSolution retour;
	bool normalSet = 0;
	const Float2 V = pancien - pactu;
	const Float2 VD = posConvexeAncien - posConvexe;
	
	short taille = forme.size();
	for (short i = 0; i < taille; i++)
	{
		Float2 sommets[2];
		sommets[0] = forme.sommets[i];
		if (i == 0)sommets[1] =forme.sommets[taille-1];
		else sommets[1] = forme.sommets[i - 1];
		Droite D = Droite(sommets[0], sommets[1]);

		if (isnan(D.A))
		{
			const float denom = V.x - VD.x;
			if (denom > 0.000001 || denom < 0.000001)
			{
				const float num = D.B - pactu.x;
				const float fact = num / denom;
				if (fact > retour.factor && fact < 1.2)
				{
					Float2 borneMin = sommets[0];
					Float2 borneMax = sommets[1];
					if (sommets[0].x > sommets[1].x)
					{
						borneMin.x = sommets[1].x;
						borneMax.x = sommets[0].x;
					}
					if (sommets[0].y > sommets[1].y)
					{
						borneMin.y = sommets[1].y;
						borneMax.y = sommets[0].y;
					}
					borneMin += VD * fact;
					borneMax += VD * fact;
					Float2 position = pactu + V * fact;
					if (position.y < borneMax.y && position.y > borneMin.y)
					{
						retour.factor = fact;
						//calcul normal
						Float2 vitesse = (V - VD)*fact;


						float angle = 0;
						float delta = (vitesse).angle();
						if (delta < -PI / 2) delta += 2 * PI;
						else if (delta > 3 * PI / 2) delta -= 2 * PI;
						if (delta > PI / 2) angle += PI;

						retour.normal = Float2(0, 1).setAngle(angle);
						normalSet = true;
					}

				}
			}
		}
		else
		{
			const float denom = V.y - VD.y + D.A * (VD.x - V.x);
			if (denom > 0.000001 || denom < 0.000001)
			{
				const float num = sommets[0].y - pactu.y + D.A * (pactu.x - sommets[0].x);
				const float fact = num / denom;
				if (fact > retour.factor && fact < 2)
				{
					
					Float2 borneMin = sommets[0];
					Float2 borneMax = sommets[1];
					if (sommets[0].x > sommets[1].x)
					{
						borneMin.x = sommets[1].x;
						borneMax.x = sommets[0].x;
					}
					if (sommets[0].y > sommets[1].y)
					{
						borneMin.y = sommets[1].y;
						borneMax.y = sommets[0].y;
					}
					borneMin += VD * fact;
					borneMax += VD * fact;
					Float2 position =  pactu+ V * fact;
					bool ignoreY = D.A < 0.00001 && D.A > -0.00001;
					if (position.x < borneMax.x && position.x > borneMin.x&&(ignoreY||( position.y < borneMax.y && position.y > borneMin.y)))
					{
						retour.factor = fact;
						//calcul normal
						Float2 vitesse = (V - VD) * fact;
						float angle = ((Float2)(sommets[0] - sommets[1])).angle() + PI / 2;
						float angle2 = vitesse.angle();
						float delta = angle - angle2;
						if (delta < -PI/2) delta += 2*PI;
						else if (delta > 3*PI/2) delta -= 2*PI;
						if (delta > PI / 2) angle += PI;

						retour.normal = Float2(0,1).setAngle(angle);
						normalSet = true;
					}
					
				}
			}
		}
	}
	if (!normalSet)
	{
		retour.factor = 0;
		Float2 normal;
		int taille = forme.size();
		for (int i = 0; i < taille; i++)
		{
			normal += forme.sommets[i];
		}
		normal /= (float)taille;
		normal = pancien - normal;
		normal.setNorm(1);
		retour.normal = normal;
	}
	return retour;
}
#pragma endregion fin de corps
#pragma region corps_visible
VisibleBody::VisibleBody()
	:RigidBody()
{
	layer = 0;
	modif_images = false;
}
VisibleBody::VisibleBody(float _masse, Float2 _position, Concave _forme, bool dynamic )
	: RigidBody(_masse, _position, _forme, dynamic)
{
	layer = 0;
	modif_images = true;
}

void VisibleBody::clear_images()
{
	images.clear();
	images_offset.clear();
}
void VisibleBody::add_images(sf::Sprite image, Float2 offset)
{
	images.push_back(image);
	images_offset.push_back(offset);
	modif_images = true;
}

void VisibleBody::afficher(sf::RenderWindow& fenetre)
{
	const int taille = images.size();
	if (modif_images)
	{
		for (int i = 0; i < taille; i++)
		{
			images[i].setPosition(position + images_offset[i]);
			modif_images = false;
		}
	}
	for (int i = 0; i < taille; i++)
	{
		fenetre.draw(images[i]);
	}
}

void VisibleBody::setPosition(Float2 new_position)
{
	modif_images = true;
	RigidBody::setPosition(new_position);
}
void VisibleBody::updatePosition(sf::Time deltaT)
{
	RigidBody::updatePosition(deltaT);
	modif_images = true;
}
void VisibleBody::resize(Float2 multiplicateur)
{

	for (int i = 0, taille = images.size(); i < taille; i++)
	{
		Float2 scale = images[i].getScale();
		scale.x *= multiplicateur.x / size.x;
		scale.y *= multiplicateur.y / size.y;
		images[i].setScale(scale);
		images_offset[i].x *= multiplicateur.x / size.x;
		images_offset[i].y *= multiplicateur.y / size.y;
	}
	RigidBody::resize(multiplicateur);
	modif_images = true;
}
#pragma endregion fin de corps_visible
