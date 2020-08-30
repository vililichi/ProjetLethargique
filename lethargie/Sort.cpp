#include "Sort.h"
#include "FichierIO.h"


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


#pragma region fonctions
Sort* importSort(std::string path, float life_time , bool is_floating , bool is_spectral , bool has_percing , bool is_dynamic)
{
	Sort* p_sort = new Sort;
	std::ifstream f;
	f.open(path);
	LireFichier(f, *p_sort);
	p_sort->is_Dynamic = is_dynamic;
	p_sort->percing = has_percing;
	p_sort->spectral = is_spectral;
	p_sort->floating = is_floating;
	if (life_time > 0) p_sort->set_lifeTime(life_time);
	return p_sort;
}
Sort* SetActorAndWorld(Sort* p_sort, Vivant* acteur, bool blacklist_player)
{
	if (blacklist_player == true)p_sort->blacklist.push_back(acteur);
	acteur->p_monde->addSort(p_sort);
	return p_sort;
}
#pragma endregion
