#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"
#include "CameraMobile.h"

int main()
{
    int itt = 0;
    sf::Clock timerFPS;
    int compteur = 0;
    Monde univers;
    univers.gravity = Float2(0, 1000);
    std::ifstream ifs;
    
    Controler mainDivine;
    mainDivine.p_joueur = univers.addJoueur();
    InfusionNova* comp1 = new InfusionNova();
    InfusionDart* comp2 = new InfusionDart();
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
    camera.zoom(0.75);


    sf::RenderWindow window(sf::VideoMode(1000,700), "Lethargie");
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
            {
                camera.setSize((float)event.size.width, (float)event.size.height);
                float factor = (float)event.size.width * (float)event.size.width + (float)event.size.height * (float)event.size.height;
                const float normal = 1700000.f;
                camera.zoom( normal/factor);
            }
        }

        mainDivine.creerControl(window);
        univers.update();

        if (itt >= 3)
        {
            camera.move(mainDivine.p_joueur->getPosition(), 10);
            window.clear();
            univers.afficher(window);

            window.setView(camera);
            window.display();
            itt = 0;
        }
        else itt++;
        compteur++;
        if (compteur == 600)
        {
            
            std::cout << 600 / timerFPS.restart().asSeconds()<<" boucles par seconde" << std::endl;
            compteur = 0;
        }
    }

    return 0;
}