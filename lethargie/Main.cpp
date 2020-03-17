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


    sf::RenderWindow window(sf::VideoMode(1900, 900), "SFML works!");

    ////////////////////////////////////////////////////////////////////////////// I N T E R F A C E ////////////////////////////////////////////////////////////////////////////////
#pragma region interfaceVar

    const int decalageX = 65;

        sf::View hud;
        hud.setCenter(0, 0);
        hud.setSize(1900, 900);

        sf::Font font;
        font.loadFromFile("Pixeled.ttf");

        ATHElement background(window); {
            background.SetBackground(sf::Vector2f(-1100, -450), sf::Vector2f(1900, 900), sf::Color(0, 0, 0, 255), 0, sf::Color(0, 0, 0, 0));
        }

        ATHElement editArea16x9(window); {
            editArea16x9.SetBackground(sf::Vector2f(-1100, -450), sf::Vector2f(1600, 900), sf::Color(0, 0, 0, 255), 3, sf::Color(90, 90, 90, 255));
            editArea16x9.SetDefaultText(sf::Vector2f(370, 430), 15, sf::Color(90, 90, 90, 255), "16/9 Ratio", font);
        }

        ATHElement editArea4x3(window); {
            editArea4x3.SetBackground(sf::Vector2f(-1100, -450), sf::Vector2f(1200, 900), sf::Color(0, 0, 0, 255), 3, sf::Color(90, 150, 90, 255));
            editArea4x3.SetDefaultText(sf::Vector2f(-25, 430), 15, sf::Color(90, 150, 90, 255), "4/3 Ratio", font);
        }

        ATHElement backgroundText(window); {
            backgroundText.SetBackground(sf::Vector2f(570, -395), sf::Vector2f(310, 3), sf::Color(230, 230, 230, 255), 0, sf::Color(0, 0, 0, 0));
            backgroundText.SetDefaultText(sf::Vector2f(580, -430), 30, sf::Color(230, 230, 230, 255), "Background", font);
        }

        ATHElement posText(window); {
            posText.SetDefaultText(sf::Vector2f(530, -380), 17, sf::Color(230, 230, 230, 255), "Position : ", font);
        }

        ATHElement posXTextfield(window); {
            posXTextfield.SetBackground(sf::Vector2f(680, -383), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            posXTextfield.SetDefaultText(sf::Vector2f(700, -380), 13, sf::Color(230, 230, 230, 255), "x", font);
            posXTextfield.SetContentText(sf::Vector2f(700, -380), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement posYTextfield(window); {
            posYTextfield.SetBackground(sf::Vector2f(680 + decalageX, -383), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            posYTextfield.SetDefaultText(sf::Vector2f(700 + decalageX, -380), 13, sf::Color(230, 230, 230, 255), "y", font);
            posYTextfield.SetContentText(sf::Vector2f(700 + decalageX, -380), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement sizeText(window); {
            sizeText.SetDefaultText(sf::Vector2f(530, -380 + 30), 17, sf::Color(230, 230, 230, 255), "Size : ", font);
        }

        ATHElement sizeXTextfield(window); {
            sizeXTextfield.SetBackground(sf::Vector2f(680, -383 + 30), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            sizeXTextfield.SetDefaultText(sf::Vector2f(700, -380 + 30), 13, sf::Color(230, 230, 230, 255), "x", font);
            sizeXTextfield.SetContentText(sf::Vector2f(700, -380 + 30), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement sizeYTextfield(window); {
            sizeYTextfield.SetBackground(sf::Vector2f(680 + decalageX, -383 + 30), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            sizeYTextfield.SetDefaultText(sf::Vector2f(700 + decalageX, -380 + 30), 13, sf::Color(230, 230, 230, 255), "y", font);
            sizeYTextfield.SetContentText(sf::Vector2f(700 + decalageX, -380 + 30), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement colorText(window); {
            colorText.SetDefaultText(sf::Vector2f(530, -380 + 30 * 2), 17, sf::Color(230, 230, 230, 255), "Color : ", font);
        }

        ATHElement colorRTextfield(window); {
            colorRTextfield.SetBackground(sf::Vector2f(680, -383 + 30 * 2), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            colorRTextfield.SetDefaultText(sf::Vector2f(700, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "r", font);
            colorRTextfield.SetContentText(sf::Vector2f(700, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement colorGTextfield(window); {
            colorGTextfield.SetBackground(sf::Vector2f(680 + decalageX, -383 + 30 * 2), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            colorGTextfield.SetDefaultText(sf::Vector2f(700 + decalageX, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "g", font);
            colorGTextfield.SetContentText(sf::Vector2f(700 + decalageX, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement colorBTextfield(window); {
            colorBTextfield.SetBackground(sf::Vector2f(680 + decalageX * 2, -383 + 30 * 2), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            colorBTextfield.SetDefaultText(sf::Vector2f(700 + decalageX * 2, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "b", font);
            colorBTextfield.SetContentText(sf::Vector2f(700 + decalageX * 2, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "", font);
        }

        ATHElement colorATextfield(window); {
            colorATextfield.SetBackground(sf::Vector2f(680 + decalageX * 3 , -383 + 30 * 2), sf::Vector2f(50, 17), sf::Color(90, 90, 90, 255), 3, sf::Color(150, 150, 150, 255));
            colorATextfield.SetDefaultText(sf::Vector2f(700 + decalageX * 3, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "a", font);
            colorATextfield.SetContentText(sf::Vector2f(700 + decalageX * 3, -380 + 30 * 2), 13, sf::Color(230, 230, 230, 255), "", font);
        }

#pragma endregion

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
                hud.setSize(event.size.width, event.size.height);
            }
        }

        mainDivine.creerControl();
        univers.update();

        camera.setCenter(mainDivine.p_joueur->getPosition());

        window.clear();
        //univers.afficher(window);

        //Créateur d'interface
        sf::Vector2i _mousePos = sf::Mouse::getPosition(window) - (sf::Vector2i)window.getSize() / 2;


          if (posXTextfield.isClicked(_mousePos, 0, 1)) {
              std::cout << posXTextfield.changeTextByUser(sf::Keyboard::Key::Enter, sf::Keyboard::Key::Backspace, hud);
          }


        //Draw Interface
        {
            window.setView(hud);

            //background
            {
                background.Draw();
                editArea16x9.Draw();
                editArea4x3.Draw();
            }

            //Right Element
            {
                backgroundText.Draw();

                posText.Draw();
                posXTextfield.Draw();
                posYTextfield.Draw();

                sizeText.Draw();
                sizeXTextfield.Draw();
                sizeYTextfield.Draw();

                colorText.Draw();
                colorRTextfield.Draw();
                colorGTextfield.Draw();
                colorBTextfield.Draw();
                colorATextfield.Draw();
            }
        }

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