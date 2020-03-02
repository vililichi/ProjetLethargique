#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"

int main()
{
    Monde univers;
    std::ifstream fs;
    fs.open("perso.txt");
    corps_visible* p_perso = univers.addDynamique(corps_visible());
    LireFichier(fs, *p_perso);
    fs.close();
    
    Convexe mur;
    mur.sommets.push_back(Float2(-50,25));
    mur.sommets.push_back(Float2(50,25));
    mur.sommets.push_back(Float2(50, -25));
    mur.sommets.push_back(Float2(-50, -25));
    Concave murColide;
    murColide.formes.push_back(mur);
    corps_visible murPhy(1,Float2(0,150),murColide,0);
    
    sf::Sprite imageMur;
    sf::Texture* textMur = GestionnaireTexture::obtenirTexture("Ressource/Texture/degrad50_100.png");
    imageMur.setTexture(*textMur);
    murPhy.add_images(imageMur, Float2(-50, -25));
    
    murPhy.resize(Float2(2, 4));

    univers.addStatique(murPhy);



    

    sf::View camera;
    camera.setCenter(0, 0);
    camera.setSize(500, 500);


    sf::RenderWindow window(sf::VideoMode(500,500), "SFML works!");

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
        const float frict = 10.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))mv += Float2(0, -puiss);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))mv += Float2(0, puiss);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))mv += Float2(-puiss, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))mv += Float2(puiss, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) p_perso->vit = Float2(0, 0);

        p_perso->forc += mv;
        p_perso->forc += p_perso->vit * -frict;


        univers.update();

        camera.setCenter(p_perso->getPosition());

        window.clear();
        univers.afficher(window);

        window.setView(camera);
        window.display();
    }

    return 0;
}