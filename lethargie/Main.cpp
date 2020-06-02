#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"
#include "Menu_Creator.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1900, 900), "SFML works!");

	sf::Clock timerFPS;
	int compteur = 0;
	Monde univers;
	univers.gravity = Float2(0, 1000);
	std::ifstream ifs;

	Controler mainDivine;
	mainDivine.p_joueur = univers.addJoueur();

	ifs.open("Ressource/Map/test");
	int error = LireFichier(ifs, univers);
	std::cout << error << '\n';

	sf::View camera;
	camera.setCenter(0, 0);
	camera.setSize(500, 500);

	Menu_Creator menu_Creator;
	menu_Creator.Start(&window);

	timerFPS.restart();
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized) {
				camera.setSize(event.size.width, event.size.height);
				menu_Creator.Resize(event.size.width, event.size.height);
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					menu_Creator.mouseWheel(event.mouseWheelScroll.delta);
				}
			}
		}

		mainDivine.creerControl();
		univers.update();

		camera.setCenter(mainDivine.p_joueur->getPosition());

		window.clear();
		//univers.afficher(window);

		//Interface Update
		menu_Creator.Update();
		menu_Creator.Draw();

		window.setView(camera);
		window.display();
		compteur++;
		if (compteur == 500)
		{

			std::cout << 500 / timerFPS.restart().asSeconds() << " boucles par seconde" << std::endl;
			compteur = 0;
		}
	}

	return 0;
}