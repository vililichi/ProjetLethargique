#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"
#include "CameraMobile.h"
#include "HUD.h"

int main()
{

    int itt = 0;
    sf::Clock timerFPS;
    int compteur = 0;
    Monde univers;
    univers.gravity = Float2(0, 1000);
    std::ifstream ifs;
    
    Controler mainDivine;
    HUD haut_display;
    mainDivine.p_joueur = univers.addJoueur();
    haut_display.p_joueur = mainDivine.p_joueur;

    Damage cout_comp1;
    cout_comp1.lumiere = 2;

    Damage cout_comp2;
    cout_comp2.lumiere = 1;
    cout_comp2.entropie = 1;

    InfusionNova* comp1 = new InfusionNova();
    InfusionDart* comp2 = new InfusionDart();

    comp1->cout = cout_comp1;
    comp2->cout = cout_comp2;

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

    sf::View view_interface;
    view_interface.setCenter(0, 0);
    view_interface.setSize(1000, 700);


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
                
                float factor = (float)event.size.width * (float)event.size.width + (float)event.size.height * (float)event.size.height;
                const float normal = 1700000.f;

                camera.setSize((float)event.size.width, (float)event.size.height);
                camera.zoom( normal/factor);

                view_interface.setSize((float)event.size.width, (float)event.size.height);
            }
        }

        mainDivine.creerControl(window);
        univers.update();

        if (itt >= 3)
        {
            camera.move(mainDivine.p_joueur->getPosition(), 10);
            window.clear(sf::Color::Cyan);


            univers.afficher(window);       //affichage du monde
            window.setView(view_interface); //changement pour camera fixe
            haut_display.afficher(window);  //affichage de l'interface
            window.setView(camera);         //retour à la camera monde


            window.display();
            itt = 0;
        }
        else itt++;
        compteur++;
        /*if (compteur == 600)
        {
            
            std::cout << 600 / timerFPS.restart().asSeconds()<<" boucles par seconde" << std::endl;
            compteur = 0;
        }*/
    }

    return 0;
}