#include <SFML/Graphics.hpp>
#include "Physic.h"

int main()
{
    int calculT = 0;
    float temps = 0;
    sf::Clock timer;
    Convexe carre;
    carre.sommets.push_back(Float2(0,0));
    carre.sommets.push_back(Float2(0, 28));
    carre.sommets.push_back(Float2(20, 28));
    carre.sommets.push_back(Float2(20, 0));
    Concave box;
    box.formes.push_back(carre);

    sf::Sprite ptitbonhomme;
    sf::Texture textBonhomme;
    textBonhomme.loadFromFile("Ressource/Texture/test Sprite.png");
    ptitbonhomme.setTexture(textBonhomme);

    corps_visible perso(1,Float2(0,0),box);
    perso.add_images(ptitbonhomme);
    
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
    corps murPhy(1,Float2(0,0),murColide,0);
    //corps murPhy2(1, Float2(50, 50), murColide, 0);

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

        const float puiss = 5000;
        const float frict = 10.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))mv += Float2(0, -puiss);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))mv += Float2(0, puiss);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))mv += Float2(-puiss, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))mv += Float2(puiss, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) perso.vit = Float2(0, 0);
        sf::Time deltaTime = timer.restart();

        perso.forc += mv;
        perso.forc += perso.vit * -frict;

        sf::Clock tempsCol;
        tempsCol.restart();
        infoColl truc = perso + murPhy;
        temps += (tempsCol.getElapsedTime() * 100.f).asMicroseconds();
        calculT++;
        if (calculT > 100000)
        {
            temps /= (100.f * calculT);
            std::cout << "temps moyen =" << temps << std::endl;
            temps = 0;
            calculT = 0;
        }

        if (truc.taille != 0)
            for (int i = 0; i < 5; i++)
            {
                murText[i].color = sf::Color::Red; 
                murText2[i].color = sf::Color::Red;
            }
        else 
            for (int i = 0; i < 5; i++)
            { 
                murText[i].color = sf::Color::Green; 
                murText2[i].color = sf::Color::Green;
            }

        /*if ((perso + murPhy2).col)
            for (int i = 0; i < 5; i++)
            {
                murText2[i].color = sf::Color::Red;
            }
        else
            for (int i = 0; i < 5; i++)
            {
                murText2[i].color = sf::Color::Green;
            }*/

        perso.updatePosition(deltaTime);
        murPhy.updatePosition(deltaTime);
        //murPhy2.updatePosition(deltaTime);

        camera.setCenter(perso.getPosition());

        window.clear();
        window.draw(murText);
        window.draw(murText2);
        perso.afficher(window);

        window.setView(camera);
        window.display();
    }

    return 0;
}