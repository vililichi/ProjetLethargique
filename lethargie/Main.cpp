#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"

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


    /*
    corps_visible* p_objet;
    p_objet = univers.addOfficialStatique("Ressource/MapItem/platform1");
    p_objet->setPosition(Float2(0, 150));
    p_objet->resize(Float2(4, 1));

    ifs.open("Ressource/MapItem/platform1");
    p_objet = univers.addNonOfficialStatique(corps_visible());
    LireFichier(ifs, *p_objet);
    p_objet->setPosition(Float2(-200, 400));
    p_objet->resize(Float2(2, 1));
    ifs.close();

    p_objet = univers.addOfficialStatique("Ressource/MapItem/platform1");
    p_objet->setPosition(Float2(200, 150));
    p_objet->resize(Float2(0.5, 5));

    std::ofstream ofs;
    ofs.open("Ressource/Map/test");
    EcrireFichier(ofs, univers);*/

    

    sf::View camera;
    camera.setCenter(0, 0);
    camera.setSize(500, 500);


    sf::RenderWindow window(sf::VideoMode(500,500), "SFML works!");

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