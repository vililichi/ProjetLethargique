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
    mur.sommets.push_back(Float2(0,50));
    mur.sommets.push_back(Float2(0,75));
    mur.sommets.push_back(Float2(100, 75));
    mur.sommets.push_back(Float2(100, 50));
    mur.sommets.push_back(Float2(50, 25));
    Convexe mur2;
    mur2.sommets.push_back(Float2(50, 100));
    mur2.sommets.push_back(Float2(50, 125));
    mur2.sommets.push_back(Float2(150, 125));
    mur2.sommets.push_back(Float2(150, 100));
    mur2.sommets.push_back(Float2(100, 75));
    Concave murColide;
    murColide.formes.push_back(mur);
    murColide.formes.push_back(mur2);
    corps_visible murPhy(1,Float2(0,0),murColide,0);

    univers.addStatique(murPhy);


    sf::VertexArray murText (sf::TrianglesFan, 5);
    for (int i = 0; i < 5; i++) { murText[i].position = mur.sommets[i]; murText[i].color = sf::Color::Green; }
    sf::VertexArray murText2(sf::TrianglesFan, 5);
    for (int i = 0; i < 5; i++) { murText2[i].position = mur.sommets[i]+sf::Vector2f(50,50); murText2[i].color = sf::Color::Green; }

    

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
        window.draw(murText);
        window.draw(murText2);
        univers.afficher(window);

        window.setView(camera);
        window.display();
    }

    return 0;
}