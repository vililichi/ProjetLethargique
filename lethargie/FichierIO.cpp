#include"FichierIO.h"

bool LireFichier(std::string path, corps_visible& contenant)
{
	std::string type;
	std::ifstream fichier(path);
	if (!fichier)return 0;//erreur

	//type de l'objet
	fichier >> type;
	if (type != "cv")return 0;//erreur
	//caractéristique
	fichier >> contenant.masse >> contenant.bounce >> contenant.friction;
	//forme
	int taillei;
	fichier >> taillei;
	Concave forme;
	for (int i = 0; i < taillei; i++)
	{
		Convexe sousForme;
		int taillej;
		fichier >> taillej;
		for (int j = 0; j < taillej; j++)
		{
			Float2 point;
			fichier >> point;
			sousForme.sommets.push_back(point);
		}
		forme.formes.push_back(sousForme);
	}
	contenant.setForme(forme);
	//image (futur)
}

bool EcrireFichier(std::string path, corps_visible& objet)
{
	std::ofstream fichier(path);
	//type de l'objet
	fichier << "cv\n";
	//caractéristique
	fichier << objet.masse << '\t' << objet.bounce << '\t' << objet.friction << '\t';
	//forme
	fichier << objet.forme.size()<<'\n';
	for (int i = 0, taillei = objet.forme.size(); i < taillei; i++)
	{
		fichier << objet.forme.formes[i].size() << '\t\t';
		for (int j = 0, taillej = objet.forme.formes[i].size();j<taillej; j++)
		{
			fichier << objet.forme.formes[i].sommets[j] << '\t';
		}
	}
	//image (futur)


}