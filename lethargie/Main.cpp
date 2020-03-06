#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"

int main()
{
    bool doGrav = false;
    sf::Clock timerFPS;
    int compteur = 0;
    Monde univers;
    std::ifstream ifs;

    ifs.open("perso.txt");
    corps_visible* p_perso = univers.addDynamique(corps_visible());
    LireFichier(ifs, *p_perso);
    ifs.close();

    corps_visible* p_objet;
    ifs.open("Ressource/MapItem/platform1");
    p_objet = univers.addStatique(corps_visible());
    LireFichier(ifs, *p_objet);
    p_objet->setPosition(Float2(0, 150));
    p_objet->resize(Float2(4, 1));
    ifs.close();

    ifs.open("Ressource/MapItem/platform1");
    p_objet = univers.addStatique(corps_visible());
    LireFichier(ifs, *p_objet);
    p_objet->setPosition(Float2(-200, 400));
    p_objet->resize(Float2(2, 1));
    ifs.close();

    ifs.open("Ressource/MapItem/platform1");
    p_objet = univers.addStatique(corps_visible());
    LireFichier(ifs, *p_objet);
    p_objet->setPosition(Float2(200, 150));
    p_objet->resize(Float2(0.5, 5));
    ifs.close();


    

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


        Float2 mv;
        float rot = 0;

        const float puiss = 100;
        const float fren = 10;
        const float fric = 0.5;
        const Float2 grav(0, 500);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))mv += Float2(0, -puiss);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))mv += Float2(0, puiss);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))mv += Float2(-puiss, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))mv += Float2(puiss, 0);
        if (mv.x * p_perso->vit.x < 1) mv.x += p_perso->vit.x * -fren;
        mv += p_perso->vit * -fric;
        if (doGrav)mv += grav;
        else doGrav = true;

        p_perso->forc += mv;


        univers.update();

        camera.setCenter(p_perso->getPosition());

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