#include"FichierIO.h"

bool LireFichier(std::ifstream& fichier, corps_visible& contenant)
{
	std::string type;
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

	return 1;
}

bool EcrireFichier(std::ofstream& fichier, corps_visible& objet)
{
	int retour = 1;
	//type de l'objet
	fichier << "cv\n";
	//caractéristique
	fichier << objet.masse << '\t' << objet.bounce << '\t' << objet.friction << '\t';
	//forme
	fichier << objet.forme.size()<<'\n';
	for (int i = 0, taillei = objet.forme.size(); i < taillei; i++)
	{
		fichier << objet.forme.formes[i].size() << '\n';
		for (int j = 0, taillej = objet.forme.formes[i].size();j<taillej; j++)
		{
			fichier << objet.forme.formes[i].sommets[j] << '\t';
		}
		fichier << '\n';
	}
	//image (futur)
	fichier << objet.images.size() << '\n';
	for (int i = 0, taillei = objet.images.size(); i < taillei; i++)
	{
		std::string nom = GestionnaireTexture::retrouverNom(objet.images[i].getTexture());
		if (nom == "")retour = 0;
		else
		{
			fichier << nom << '\t'
				<< objet.images[i].getRotation << '\t'
				<< objet.images[i].getScale << '\t'
				<< objet.images_offet[i] << '\n';

		}
	}

	return retour;
}

std::vector<textureNommee> GestionnaireTexture::listeTexture;

sf::Texture* GestionnaireTexture::obtenirTexture(std::string nom)
{
	sf::Texture* retour = NULL;
	for (int i = 0, taille = listeTexture.size(); i < taille; i++)
	{
		if (listeTexture[i].nom == nom)
		{
			retour = &listeTexture[i].texture;
			break;
		}
	}

	if (!retour)
	{
		sf::Texture nouvelleTexture;
		if (nouvelleTexture.loadFromFile(nom))
		{
			textureNommee ajout;
			ajout.nom = nom;
			ajout.texture = nouvelleTexture;
			listeTexture.push_back(ajout);
			retour = &listeTexture[listeTexture.size-1].texture;
		}
	}
	return retour;
}

std::string GestionnaireTexture::retrouverNom(const sf::Texture* adresse)
{
	std::string retour("");
	for (int i = 0, taille = listeTexture.size(); i < taille; i++)
	{
		if (&listeTexture[i].texture == adresse)
		{
			retour = listeTexture[i].nom;
			break;
		}
	}
	return retour;
}

void GestionnaireTexture::clear()
{
	listeTexture.clear();
}