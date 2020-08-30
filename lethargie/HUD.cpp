#include "HUD.h"

HUD::HUD()
{
	//recherche des textures
	ombre =   GestionnaireTexture::obtenirTexture("Ressource/Texture/Orbe/Ombre.png");
	lumiere = GestionnaireTexture::obtenirTexture("Ressource/Texture/Orbe/Lumiere.png");
	vide =    GestionnaireTexture::obtenirTexture("Ressource/Texture/Orbe/Vide.png");
	
	//initialisation des pointeur
	unsigned short int nbr_lum = 0;
	unsigned short int nbr_omb = 0;

	ombres = new sf::Sprite[0];
	lumieres = new sf::Sprite[0];

	act_lum = 0;
	act_omb = 0;

	taille = sf::Vector2f(0,0);

	p_joueur = NULL;
}

HUD::~HUD()
{
	delete[] lumieres;
	delete[] ombres;
}

void HUD::afficher(sf::RenderWindow& fenetre)
{
	bool changement = false;
	//Caractéristique du joueur
	if (!p_joueur) return; //s'il n'y a aucun joueur, il n'y a rien à afficher

	if (p_joueur->lumiereMax != nbr_lum) //mauvaise taille pour la barre de lumiere
	{
		delete[] lumieres;
		nbr_lum = p_joueur->lumiereMax;
		lumieres = new sf::Sprite[nbr_lum];
		changement = true;
	}

	if (p_joueur->ombreMax != nbr_omb) //mauvaise taille pour la barre d'ombre
	{
		delete[] ombres;
		nbr_omb = p_joueur->ombreMax;
		ombres = new sf::Sprite[nbr_omb];
		changement = true;
	}

	//caractéristique de l'écran

	sf::View actual_view = fenetre.getView();
	sf::Vector2f view_size = actual_view.getSize();
	if (taille != view_size)
	{
		std::cout << "changement de taille : " << view_size.x << ";" << view_size.y << std::endl;
		taille = view_size;
		changement = true;
	}

	//réajustement
	if (changement)
	{
		std::cout << "ajustement";
		act_lum = p_joueur->getLumiere();
		act_omb = p_joueur->getOmbre();

		//ccalcul des tailles
		int nbr_elem = nbr_lum;
		if (nbr_omb > nbr_lum) nbr_elem = nbr_omb;
		
		int h_max = taille.y / 10;
		int l_max = taille.x / (3*nbr_omb);
		int cote = h_max;
		if (l_max < h_max) cote = l_max;

		float scale = ((float)cote) / 18.f;
		std::cout << " taille: " << scale << std::endl;

		for (short i = 0; i < nbr_lum; i++)
		{
			int posX = taille.x /2 - 10 - (cote * (i + 1));
			int posY = 20 - taille.y/2;
			lumieres[i].setPosition(sf::Vector2f(posX, posY));
			lumieres[i].setScale(scale,scale);

			if (i < act_lum) lumieres[i].setTexture(*lumiere);
			else lumieres[i].setTexture(*vide);
		}
		for (short i = 0; i < nbr_omb; i++)
		{
			int posX = 10 + (cote * i) - taille.x/2;
			int posY = 20 - taille.y / 2;
			ombres[i].setPosition(sf::Vector2f(posX, posY));
			ombres[i].setScale(scale, scale);

			if (i < act_omb) ombres[i].setTexture(*ombre);
			else ombres[i].setTexture(*vide);
		}
	}

	//changmenet d'état du joueur
	if (act_lum != p_joueur->getLumiere())
	{
		act_lum = p_joueur->getLumiere();
		for (short i = 0; i < nbr_lum; i++)
		{
			if (i < act_lum) lumieres[i].setTexture(*lumiere);
			else lumieres[i].setTexture(*vide);
		}
	}
	if (act_omb != p_joueur->getOmbre())
	{
		act_omb = p_joueur->getOmbre();
		for (short i = 0; i < nbr_omb; i++)
		{
			if (i < act_omb) ombres[i].setTexture(*ombre);
			else ombres[i].setTexture(*vide);
		}
	}

	//affichage
	for (short i = 0; i < nbr_lum; i++)
	{
		fenetre.draw(lumieres[i]);
	}
	for (short i = 0; i < nbr_omb; i++)
	{
		fenetre.draw(ombres[i]);
	}
}