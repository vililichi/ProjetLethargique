#include "Sort.h"


Sort::Sort() : VisibleBody()
{
	lifeTime = 0;
	has_lifeTime = false;

	whiteliste_only = false;

	percing = false;
	spectral = false;
	floating = false;

	end = false;
	alpha0Set = false;
	endAnim = 0.f;
	endTime = 0.2f;
}
float Sort::get_lifeTime()
{
	return lifeTime;
}
float Sort::set_lifeTime(float new_lifeTime)
{
	lifeTime = new_lifeTime;
	has_lifeTime = true;
	return new_lifeTime;
}
void Sort::update(float sec)
{
	if (has_lifeTime)
	{
		lifeTime -= sec;
		if (lifeTime < 0) end = true;
	}
	if (floating) forc = Float2(0, 0);
	if (is_Dynamic)updatePosition(sec);

	if (end)ending(sec);
}
void Sort::ending(float sec)
{
	endAnim += sec / endTime;
	for (int i = 0, taille = images.size(); i < taille; i++)
	{
		
		sf::Color Couleur = images[i].getColor();

		if (!alpha0Set)alpha0.push_back(Couleur.a);
		

		Couleur.a = (sf::Uint8)(alpha0[i] - alpha0[i] * endAnim);
		images[i].setColor(Couleur);
	}
	alpha0Set = true;
	if (endAnim >= 1)
	{
		to_destroy = true;
	}
}
void Sort::collideWall(RigidBody& wall)
{
	if (spectral || end)return;
	if (collide(wall).taille > 0) end = true;
}
void Sort::collideVivant(Vivant& cible)
{
	if (end)return;
	//whitelist et blacklist
	if (whiteliste_only)
	{
		bool out = true;
		for (int i = 0, taille = whitelist.size(); i < taille; i++)
		{
			if (whitelist[i] == &cible) out = false;
		}
		if (out)return;
	}
	for (int i = 0, taille = blacklist.size();i < taille; i++)
	{
		if (blacklist[i] == &cible) return;
	}
	//collision
	if (collide(cible).taille > 0)
	{
		cible.attack(dommage);
		blacklist.push_back(&cible);
		if (!percing)end = true;
	}
}
