#include"FichierIO.h"
#pragma region fichiers corps
int LireFichier(std::ifstream& fichier, VisibleBody& contenant)
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
int EcrireFichier(std::ofstream& fichier, VisibleBody& objet)
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
				<< objet.images_offset[i] << '\n';

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
	clear();
}
VisibleBody* ProtoMonde::addOfficialStatique(std::string nom)
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
VisibleBody* ProtoMonde::addOfficialDynamique(std::string nom)
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
VisibleBody* ProtoMonde::addNonOfficialStatique(VisibleBody new_corps)
{
	VisibleBody* p_new_corps = new VisibleBody(new_corps);
	p_new_corps->is_Dynamic = false;
	p_new_corps->setMonde(this);
	statiques.push_back(p_new_corps);
	non_official_statiques.push_back(p_new_corps);
	return p_new_corps;
}
VisibleBody* ProtoMonde::addNonOfficialDynamique(VisibleBody new_corps)
{
	VisibleBody* p_new_corps = new VisibleBody(new_corps);
	p_new_corps->is_Dynamic = true;
	p_new_corps->setMonde(this);
	dynamiques.push_back(p_new_corps);
	non_official_dynamiques.push_back(p_new_corps);
	return p_new_corps;
}
 void ProtoMonde::clear()
{
	
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
	for (int i = 0, taille = non_official_dynamiques.size(); i < taille; i++)
	{
		if (non_official_dynamiques[i] != NULL)
		{
			delete non_official_dynamiques[i];
		}
	}
	for (int i = 0, taille = non_official_statiques.size(); i < taille; i++)
	{
		if (non_official_statiques[i] != NULL)
		{
			delete non_official_statiques[i];
		}
	}
	official_dynamiques.clear();
	official_statiques.clear();
	non_official_dynamiques.clear();
	non_official_statiques.clear();
	statiques.clear();
	dynamiques.clear();
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
			//listeTexture[index].texture->setSmooth(true);
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
#pragma region GestionnaireFont
std::vector<fontNommee> GestionnaireFont::listeFont;
sf::Font* GestionnaireFont::obtenirFont(std::string nom)
{
	sf::Font* retour = NULL;
	for (int i = 0, taille = listeFont.size(); i < taille; i++)
	{
		if (listeFont[i].nom == nom)
		{
			retour = listeFont[i].font;
			break;
		}
	}

	if (!retour)
	{
		unsigned int index = listeFont.size();
		listeFont.push_back(fontNommee());
		listeFont[index].font = new sf::Font;
		if (listeFont[index].font->loadFromFile(nom))
		{
			listeFont[index].nom = nom;
			retour = listeFont[index].font;
		}
		else
		{
			delete listeFont[index].font;
			listeFont.pop_back();
		}
	}
	return retour;
}

std::string GestionnaireFont::retrouverNom(const sf::Font* adresse)
{
	std::string retour("");
	for (int i = 0, taille = listeFont.size(); i < taille; i++)
	{
		if (listeFont[i].font == adresse)
		{
			retour = listeFont[i].nom;
			break;
		}
	}
	return retour;
}

void GestionnaireFont::clear()
{
	for (int i = 0, taille = listeFont.size(); i < taille; i++)
	{
		delete listeFont[i].font;
	}
	listeFont.clear();
}

#pragma endregion
#pragma region fichiers monde
int LireFichier(std::ifstream& fichier, ProtoMonde& contenant)
{
	int retour = REUSSITE;
	int taille;
	std::string type;
	if (!fichier)return ERREUR_OUVERTURE;//erreur

	//type de l'objet
	fichier >> type;
	if (type != "md")return ERREUR_FORMAT;//erreur
	//caractéristique
	fichier >> contenant.gravity;
	//statique officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		std::string nom;
		VisibleBody* p_objet;
		Float2 position;
		Float2 size;

		fichier >> nom;
		p_objet = contenant.addOfficialStatique(nom);
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);
	}
	//statique non officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		VisibleBody* p_objet;
		Float2 position ;
		Float2 size;

		p_objet = contenant.addNonOfficialStatique(VisibleBody());
		LireFichier(fichier, *p_objet);
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);
	}
	//dynamique officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		std::string nom;
		VisibleBody* p_objet;
		Float2 position;
		Float2 size;

		fichier >> nom;
		p_objet = contenant.addOfficialDynamique(nom);
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);

	}
	//dynamique non officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		VisibleBody* p_objet;
		Float2 position;
		Float2 size;

		p_objet = contenant.addNonOfficialDynamique(VisibleBody());
		LireFichier(fichier, *p_objet);
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);
	}
	return retour;
}
int LireFichier(std::ifstream& fichier, Monde& contenant)
{
	int retour = REUSSITE;
	int taille;
	std::string type;
	if (!fichier)return ERREUR_OUVERTURE;//erreur

	//type de l'objet
	fichier >> type;
	if (type != "md")return ERREUR_FORMAT;//erreur
	//caractéristique
	fichier >> contenant.gravity;
	//statique officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		std::ifstream ifs;
		VisibleBody* p_objet;
		std::string nom;
		Float2 position;
		Float2 size;

		fichier >> nom;
		ifs.open(nom);
		if (!ifs) return ERREUR_OUVERTURE;
		p_objet = contenant.addStatique(VisibleBody());
		LireFichier(ifs, *p_objet);
		ifs.close();
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);
	}
	//statique non officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		VisibleBody* p_objet;
		Float2 position;
		Float2 size;

		p_objet = contenant.addStatique(VisibleBody());
		LireFichier(fichier, *p_objet);
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);
	}
	//dynamique officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		std::ifstream ifs;
		VisibleBody* p_objet;
		std::string nom;
		Float2 position;
		Float2 size;

		fichier >> nom;
		ifs.open(nom);
		if (!ifs) return ERREUR_OUVERTURE;
		p_objet = contenant.addDynamique(VisibleBody());
		LireFichier(ifs, *p_objet);
		ifs.close();
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);

	}
	//dynamique non officiel
	fichier >> taille;
	for (int i = 0; i < taille; i++)
	{
		std::ifstream ifs;
		VisibleBody* p_objet;
		Float2 position;
		Float2 size;

		p_objet = contenant.addDynamique(VisibleBody());
		LireFichier(fichier, *p_objet);
		fichier >> position;
		fichier >> size;
		p_objet->setPosition(position);
		p_objet->resize(size);
	}
	return retour;
}
int EcrireFichier(std::ofstream& fichier, ProtoMonde& monde)
{
	int retour = REUSSITE;
	if (!fichier) return ERREUR_OUVERTURE;
	//type de l'objet
	fichier << "md\n";
	//caractéristique
	fichier << monde.gravity << '\n';
	//statique officiel
	fichier << monde.official_statiques.size() << '\n';
	for (int i = 0, taille = monde.official_statiques.size(); i < taille; i++)
	{
		fichier << monde.official_statiques[i]->nom << '\n';
		fichier << monde.official_statiques[i]->corps.getPosition() << '\n';
		fichier << monde.official_statiques[i]->corps.getSize() << '\n';
	}
	//statique non officiel
	fichier << monde.non_official_statiques.size() << '\n';
	for (int i = 0, taille = monde.non_official_statiques.size(); i < taille; i++)
	{
		int erreur = EcrireFichier(fichier, *monde.non_official_statiques[i]);
		fichier << monde.non_official_statiques[i]->getPosition() << '\n';
		fichier << monde.non_official_statiques[i]->getSize() << '\n';
		if (erreur != 1)
		{
			fichier << "/!\\ erreur " << erreur << " /!\\" << '\n';
			return erreur;
		}
	}
	//dynamique officiel
	fichier << monde.official_dynamiques.size() << '\n';
	for (int i = 0, taille = monde.official_dynamiques.size(); i < taille; i++)
	{
		fichier << monde.official_dynamiques[i]->nom << '\n';
		fichier << monde.official_dynamiques[i]->corps.getPosition() << '\n';
		fichier << monde.official_dynamiques[i]->corps.getSize() << '\n';
	}
	//dynamique non officiel
	fichier << monde.non_official_dynamiques.size() << '\n';
	for (int i = 0, taille = monde.non_official_dynamiques.size(); i < taille; i++)
	{
		int erreur = EcrireFichier(fichier, *monde.non_official_dynamiques[i]);
		fichier << monde.non_official_dynamiques[i]->getPosition() << '\n';
		fichier << monde.non_official_dynamiques[i]->getSize() << '\n';
		if (erreur != 1)
		{
			fichier << "/!\\ erreur " << erreur << " /!\\" << '\n';
			return erreur;
		}
	}
	return retour;
}
#pragma endregion
