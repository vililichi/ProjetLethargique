
#include "Physic.h"
#include "Joueur.h"
#include "FichierIO.h"
#pragma region constantes
//id images
const int bras1 = 0;
const int corps = 1;
const int tete = 2;
const int bras2 = 3;
const int pied1 = 4;
const int pied2 = 5;
//id action
const int jump = 0;
const int action1 = 1;
const int action2 = 2;
const int dash = 3;
const int jumpMem = 4;
//id animation
const int idle = 0;
const int marche = 1;

const float actionTime[nbrAction] = { 0.3, 0.7,0.7,1,0.2 };
const float animTime[nbrAnim] = {1,0.5};

const float puiss = 300;
const float fren = 10;
const float fric = 0.5;
const Float2 grav(0, 500);
#pragma endregion
#pragma region joueur
	Joueur::Joueur() : Vivant()
	{
		std::ifstream ifs;
		ifs.open("Ressource/MapItem/Vivant/perso.txt");
		LireFichier(ifs, *this);
		ifs.close();

		images_init_offset.clear();
		for (int i = 0, taille = images_offet.size(); i < taille; i++)
		{
			images_init_offset.push_back(images_offet[i]);
		}
	
		for (int i = 0; i < nbrAction; i++)
		{
			actionPret[i] = true;
			actionTimer[i] = 0;
		}
		for (int i = 0; i < nbrAnim; i++)
		{
			animActif[i] = false;
			animEtat[i] = 0;
		}
	}
	void Joueur::update(sf::Time deltaT)
	{
		float sec = deltaT.asSeconds();
		//aucune action
		if ((!actionFutur.gauche && !actionFutur.droite)||(actionFutur.gauche && actionFutur.droite))
		{
			animActif[idle] = true;
		}
		else
		{
			animActif[marche] = true;
		}

		//deplacements latéraux
		Float2 actionForce;
		if (actionFutur.gauche)actionForce += Float2( -puiss,0 );
		if (actionFutur.droite)actionForce += Float2( puiss,0 );
		if (actionForce.x * vit.x < 1) actionForce.x += vit.x * -fren;
		forc += actionForce;

		//dash et jump
		Float2 actionVit;
		if( actionFutur.jump && actionPret[jump])
		{
			actionPret[jumpMem] = true;
			actionTimer[jumpMem] = actionTime[jumpMem];
		}
		if (actionFutur.dash && actionPret[dash])
		{
			if (actionForce.norm2() > puiss / 10)
			{
				actionVit += actionForce;
				actionPret[dash] = false;
				actionTimer[dash] = actionTime[dash];
			}
		}
		vit += actionVit;

		//mise à jour des timers
		for (int i = 0; i < nbrAction; i++)
		{
			if (i != jumpMem)
			{
				if (!actionPret[i])
				{
					actionTimer[i] -= sec;
					if (actionTimer[i] <= 0)
					{
						actionPret[i] = true;
					}
				}
			}
			else
			{
				if (actionPret[i])
				{
					actionTimer[i] -= sec;
					if (actionTimer[i] <= 0)
					{
						actionPret[i] = false;
					}
				}
			}
		}

		//mise à jour des animations
		for (int i = 0; i < nbrAnim; i++)
		{
			if (animActif[i])
			{
				animEtat[i] += sec / animTime[i];
				if (animEtat[i] > 1)
				{
					animEtat[i] = 0;
					animActif[i] = false;
				}
			}
		}

		//animations
		//tete
		images_offet[tete] = images_init_offset[tete] + Float2(0, 0.7*sin(2 * PI * animEtat[idle]));

		//bras1
		images_offet[bras1] = images_init_offset[bras1] + 
			Float2(
				sin(PI * (2 * animEtat[marche] + 1)), 
				sin(PI*(2 * animEtat[idle] + 1)));

		//bras2
		images_offet[bras2] = images_init_offset[bras2] + 
			Float2(
				sin(PI * (2 * animEtat[marche] )),
				sin(PI * (2 * animEtat[idle] + 1)));
		
		//pied1
		images_offet[pied1] = images_init_offset[pied1] + 
			Float2(
				sin(PI * (2 * animEtat[marche]+1)),
				0.5 * sin(PI * (2 * animEtat[marche] + 1)));

		//pied2
		images_offet[pied2] = images_init_offset[pied2] +
			Float2(
				sin(PI * (2 * animEtat[marche])),
				0.5 * sin(PI * (2 * animEtat[marche])));

		//update normale
		modif_images = true;

		Vivant::update(deltaT);
	}
	bool Joueur::collideJump(infoColl collision)
	{
		if (collision.grounded && actionPret[jumpMem] && actionPret[jump])
		{
			actionPret[jumpMem] = false;
			actionPret[jump] = false;
			actionTimer[jump] = actionTime[jump];
			Float2 normalGrav = ((Monde*)p_monde)->gravity;
			normalGrav.setNorm(1);
			Float2 modVit = 2.f * collision.groundDir - normalGrav;
			vit += modVit.setNorm(puiss * 1.3f);
			return true;
		}
		else return false;
	}

#pragma endregion
#pragma region controler
	Controler::Controler()
	{
		p_joueur = NULL;
	}
	void Controler::creerControl()
	{
		controlInfo controlInput;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))controlInput.haut = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))controlInput.bas = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))controlInput.gauche = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))controlInput.droite = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))controlInput.jump = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))controlInput.dash = true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))controlInput.action1 = true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))controlInput.action2 = true;
		p_joueur->actionFutur = controlInput;
	}
#pragma endregion
