#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"
#include "ATH.h"

int main()
{

    sf::Clock timerFPS;
    int compteur = 0;
    Monde univers;
    univers.gravity = Float2(0, 1000);
    std::ifstream ifs;
    
    Controler mainDivine;
    mainDivine.p_joueur = univers.addJoueur();

    ifs.open("Ressource/Map/test");
    int error = LireFichier(ifs, univers);
    std::cout << error<< '\n';

    sf::View camera;
    camera.setCenter(0, 0);
    camera.setSize(500, 500);


    sf::RenderWindow window(sf::VideoMode(500,500), "SFML works!");

    //interface
    sf::View hud;
    hud.setCenter(0, 0);
    hud.setSize(500, 500);

    Button bouton(window, 0, 0, 100, 40, 5);

    timerFPS.restart();
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                camera.setSize(event.size.width, event.size.height);
        }

        mainDivine.creerControl();
        univers.update();

        camera.setCenter(mainDivine.p_joueur->getPosition());

        window.clear();
        univers.afficher(window);

        //interface
        window.setView(hud);
        bouton.Draw();

        window.setView(camera);
        window.display();
        compteur++;
        if (compteur == 10000)
        {
            
            std::cout << 10000 / timerFPS.restart().asSeconds()<<" boucles par seconde" << std::endl;
            compteur = 0;
        }
    }

    return 0;
}