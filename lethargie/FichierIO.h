#pragma once
#include "Monde.h"
#include <iostream>
#include <fstream>
#include <string>
/*

bool LireFichier(std::string path, corps_visible& contenant);
bool EcrireFichier(std::string path, corps_visible& objet);

prototypes dans Physic.h
*/

struct textureNommee
{
	std::string nom;
	sf::Texture* texture;
};

struct official_corps_visible
{
	std::string nom;
	corps_visible corps;
};

class GestionnaireTexture
{
protected:
	static std::vector<textureNommee> listeTexture;
public :
	static sf::Texture* obtenirTexture(std::string nom);
	static std::string retrouverNom(const sf::Texture* adresse);
	static void clear();
};

class ProtoMonde : Monde
{
public:
	ProtoMonde();
	~ProtoMonde();

	virtual void clear();

	std::vector<official_corps_visible*> official_dynamiques;
	std::vector<official_corps_visible*> official_statiques;

	corps_visible* addOfficialStatique(std::string nom);
	corps_visible* addOfficialDynamique(std::string nom);
};
