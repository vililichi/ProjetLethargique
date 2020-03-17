#include"FichierIO.h"
#pragma region fichiers corps
int LireFichier(std::ifstream& fichier, corps_visible& contenant)
{
	int retour = REUSSITE;
	std::string type;
	if (!fichier)return ERREUR_OUVERTURE;//erreur

	//type de l'objet
	fichier >> type;
	if (type != "cv")return ERREUR_FORMAT;//erreur
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
	//image
	fichier >> taillei;
	for (int i = 0; i < taillei; i++)
	{
		sf::Sprite image;
		std::string fichierImage;
		float rot;
		Float2 scale;
		Float2 offSet;
		fichier >> fichierImage;
		fichier >> rot;
		fichier >> scale;
		fichier >> offSet;
		sf::Texture* p_text = GestionnaireTexture::obtenirTexture(fichierImage);
		if (!p_text) retour = ERREUR_OUVERTURE_TEXTURE;
		else
		{
			image.setTexture(*p_text);
			image.setRotation(rot);
			image.setScale(scale);
			contenant.add_images(image, offSet);
		}
	}

	return retour;
}
int EcrireFichier(std::ofstream& fichier, corps_visible& objet)
{
	int retour = REUSSITE;
	if (!fichier) return ERREUR_OUVERTURE;
	//type de l'objet
	fichier << "cv\n";
	//caractéristique
	fichier << objet.masse << '\t' << objet.bounce << '\t' << objet.friction << '\n';
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
	//image
	fichier << objet.images.size() << '\n';
	for (int i = 0, taillei = objet.images.size(); i < taillei; i++)
	{
		std::string nom = GestionnaireTexture::retrouverNom(objet.images[i].getTexture());
		if (nom == "")retour = ERREUR_OUVERTURE_TEXTURE;
		else
		{
			fichier << nom << '\t'
				<< objet.images[i].getRotation() << '\t'
				<< (Float2)objet.images[i].getScale() << '\t'
				<< objet.images_offet[i] << '\n';

		}
	}

	return retour;
}
#pragma endregion
#pragma region ProtoMonde
ProtoMonde::ProtoMonde() : Monde()
{
	clear();
}
ProtoMonde::~ProtoMonde()
{
	Monde::~Monde();
	clear();
}
corps_visible* ProtoMonde::addOfficialStatique(std::string nom)
{
	official_corps_visible* p_new_corps = new official_corps_visible;
	official_statiques.push_back(p_new_corps);
	statiques.push_back(&p_new_corps->corps);
	std::ifstream ifs;
	ifs.open(nom);
	LireFichier(ifs, p_new_corps->corps);
	ifs.close();
	p_new_corps->corps.is_Dynamic = false;
	p_new_corps->corps.setMonde(this);
	p_new_corps->nom = nom;
	return &p_new_corps->corps;
}
corps_visible* ProtoMonde::addOfficialDynamique(std::string nom)
{
	official_corps_visible* p_new_corps = new official_corps_visible;
	official_dynamiques.push_back(p_new_corps);
	dynamiques.push_back(&p_new_corps->corps);
	std::ifstream ifs;
	ifs.open(nom);
	LireFichier(ifs, p_new_corps->corps);
	ifs.close();
	p_new_corps->corps.is_Dynamic = true;
	p_new_corps->corps.setMonde(this);
	p_new_corps->nom = nom;
	return &p_new_corps->corps;
}
corps_visible* ProtoMonde::addNonOfficialStatique(corps_visible new_corps)
{
	corps_visible* p_new_corps = new corps_visible(new_corps);
	p_new_corps->is_Dynamic = false;
	p_new_corps->setMonde(this);
	statiques.push_back(p_new_corps);
	non_official_statiques.push_back(p_new_corps);
	return p_new_corps;
}
corps_visible* ProtoMonde::addNonOfficialDynamique(corps_visible new_corps)
{
	corps_visible* p_new_corps = new corps_visible(new_corps);
	p_new_corps->is_Dynamic = true;
	p_new_corps->setMonde(this);
	dynamiques.push_back(p_new_corps);
	non_official_dynamiques.push_back(p_new_corps);
	return p_new_corps;
}
 void ProtoMonde::clear()
{
	 Monde::clear();
	for (int i = 0, taille = official_dynamiques.size(); i < taille; i++)
	{
		if (official_dynamiques[i] != NULL)
		{
			delete official_dynamiques[i];
		}
	}
	for (int i = 0, taille = official_statiques.size(); i < taille; i++)
	{
		if (official_statiques[i] != NULL)
		{
			delete official_statiques[i];
		}
	}
	official_dynamiques.clear();
	official_statiques.clear();
}
#pragma endregion
#pragma region GestionnaireTexture
std::vector<textureNommee> GestionnaireTexture::listeTexture;
sf::Texture* GestionnaireTexture::obtenirTexture(std::string nom)
{
	sf::Texture* retour = NULL;
	for (int i = 0, taille = listeTexture.size(); i < taille; i++)
	{
		if (listeTexture[i].nom == nom)
		{
			retour = listeTexture[i].texture;
			break;
		}
	}

	if (!retour)
	{
		unsigned int index = listeTexture.size();
		listeTexture.push_back(textureNommee());
		listeTexture[index].texture = new sf::Texture;
		if (listeTexture[index].texture->loadFromFile(nom))
		{
			listeTexture[index].nom = nom;
			retour = listeTexture[index].texture;
		}
		else
		{
			delete listeTexture[index].texture;
			listeTexture.pop_back();
		}
	}
	return retour;
}

std::string GestionnaireTexture::retrouverNom(const sf::Texture* adresse)
{
	std::string retour("");
	for (int i = 0, taille = listeTexture.size(); i < taille; i++)
	{
		if (listeTexture[i].texture == adresse)
		{
			retour = listeTexture[i].nom;
			break;
		}
	}
	return retour;
}

void GestionnaireTexture::clear()
{
	for (int i = 0, taille = listeTexture.size(); i < taille; i++)
	{
		delete listeTexture[i].texture;
	}
	listeTexture.clear();
}

#pragma endregion
#pragma region fichiers monde
int LireFichier(std::ifstream& fichier, ProtoMonde& contenant)
{
	return 0;
}
int LireFichier(std::ifstream& fichier, Monde& contenant)
{
	return 0;
}
int EcrireFichier(std::ofstream& fichier, ProtoMonde& monde)
{
	return 0;
}
#pragma endregion