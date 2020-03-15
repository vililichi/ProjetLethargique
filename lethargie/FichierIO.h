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
class GestionnaireTexture
{
protected:
	static std::vector<textureNommee> listeTexture;
public :
	static sf::Texture* obtenirTexture(std::string nom);
	static std::string retrouverNom(const sf::Texture* adresse);
	static void clear();
};
