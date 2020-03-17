
#include "Joueur.h"
#include "FichierIO.h"
#pragma region constantes
const int jump = 0;
const int action1 = 1;
const int action2 = 2;
const int dash = 3;
const int jumpMem = 4;

const float actionTime[nbrAction] = { 0.3, 0.7,0.7,1,0.2 };

const float puiss = 300;
const float fren = 10;
const float fric = 0.5;
const Float2 grav(0, 500);
#pragma endregion
#pragma region joueur
	Joueur::Joueur() : Vivant()
	{
		std::ifstream ifs;
		ifs.open("perso.txt");
		LireFichier(ifs, *this);
		ifs.close();
	
		for (int i = 0; i < nbrAction; i++)
		{
			actionPret[i] = true;
			actionTimer[i] = 0;
		}
	}
	void Joueur::update(sf::Time deltaT)
	{
		float sec = deltaT.asSeconds();

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

		//update normale
		Vivant::update(deltaT);

	}
	infoColl Joueur::operator + (corps& c)
	{
		infoColl collision =  corps::operator +(c) ;
		if (collision.grounded && actionPret[jumpMem] && actionPret[jump])
		{
			actionPret[jumpMem] = false;
			actionPret[jump] = false;
			actionTimer[jump] = actionTime[jump];
			vit += puiss  * collision.groundDir;
		}
		return collision;
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



	

