#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"
#include "CameraMobile.h"

int main()
{
    sf::Clock timerFPS;
    int compteur = 0;
    Monde univers;
    univers.gravity = Float2(0, 1000);
    std::ifstream ifs;
    
    Controler mainDivine;
    mainDivine.p_joueur = univers.addJoueur();
    InfusionDart* comp1 = new InfusionDart();
    InfusionSphere* comp2 = new InfusionSphere();
    Arme* arme = new Arme();
    arme->setCompetence(comp1, 1);
    arme->setCompetence(comp2, 2);
    mainDivine.p_joueur->equiper(arme);

    ifs.open("Ressource/Map/test");
    int error = LireFichier(ifs, univers);
    std::cout << error<< '\n';

    CameraMobile camera;
    camera.setCenter(0, 0);
    camera.setSize(1000, 700);


    sf::RenderWindow window(sf::VideoMode(1000,700), "SFML works!");
    window.setVerticalSyncEnabled(true);

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

        mainDivine.creerControl(window);
        univers.update();

        camera.move(mainDivine.p_joueur->getPosition(),10);

        window.clear();
        univers.afficher(window);

        window.setView(camera);
        window.display();
        compteur++;
        if (compteur == 180)
        {
            
            std::cout << 180 / timerFPS.restart().asSeconds()<<" boucles par seconde" << std::endl;
            compteur = 0;
        }
    }

    return 0;
}