#include <SFML/Graphics.hpp>
#include "Monde.h"
#include "FichierIO.h"
#include "Joueur.h"
#include "ATH.h"
//#include "Menu_Creator.h"

sf::RenderWindow window(sf::VideoMode(1900, 900), "Menu Creator Application");

////Menu_Creator////
sf::View view(sf::Vector2f(0, 0), sf::Vector2f(1900, 900));

sf::Vector2i _deltaMousePos, _oldMousePos, _mousePos;

ATHElement menu_Creator_Origin(window, view, "");
ATHElement *globalBackground, *menuCreator, *menu16x9;

int selectedElement = 0;

void Draw();
void Resize(unsigned int _newSizeX, unsigned int _newSizeY);
void LoadFromATHElement(int _child);

float ToFloat(std::string _string);
sf::Vector2f ToVector2f(std::vector<ATHElement> _childs, int _firstElement);
sf::Color ToColor(std::vector<ATHElement> _childs, int _firstElement);

void FillContentText(sf::Vector2f _vector2f, std::vector<ATHElement>& _tochilds, int _firstElement);
void FillContentText(sf::Color _color, std::vector<ATHElement>& _tochilds, int _firstElement);
void FillContentText(float _float, ATHElement& _toElement);
void FillContentText(const sf::Font* _font, ATHElement& _toElement);
void FillContentText(std::string _string, ATHElement& _toElement);


int main()
{
	////////START////////
	//Var and function run one time
	sf::Clock timerFPS;
	int compteur = 0;
	Monde univers;
	univers.gravity = Float2(0, 1000);
	std::ifstream ifs;

	Controler mainDivine;
	mainDivine.p_joueur = univers.addJoueur();

	ifs.open("Ressource/Map/test");
	int error = LireFichier(ifs, univers);
	std::cout << error << '\n';

	sf::View camera;
	camera.setCenter(0, 0);
	camera.setSize(500, 500);

	//menu_Creator_Origin.AddElement();
	menu_Creator_Origin.AddElement("", 3);

	globalBackground = &menu_Creator_Origin.childs[0];
	menuCreator = &menu_Creator_Origin.childs[1];
	menu16x9 = &menu_Creator_Origin.childs[2];

	globalBackground->SetBackground(sf::Vector2f(-950, -450), sf::Vector2f(1900, 900), sf::Color(20, 0, 20, 255), 0, sf::Color::Transparent);

	menuCreator->Load("Menu_Creator.txt");
	menu16x9->AddElement();

	////////UPDATE////////
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
				Resize(event.size.width, event.size.height);
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					const float scaleFactor = 1 + event.mouseWheelScroll.delta * 0.05f;
					menu16x9->Scale(scaleFactor, scaleFactor);
				}
			}
		}

		mainDivine.creerControl();
		univers.update();

		camera.setCenter(mainDivine.p_joueur->getPosition());

		window.clear();
		//univers.afficher(window);

		//Interface Update
		_oldMousePos = _mousePos;
		_mousePos = sf::Mouse::getPosition(window) - (sf::Vector2i)window.getSize() / 2;
		_deltaMousePos = _mousePos - _oldMousePos;
		const int textField[] = { 5, 6, 8, 9, 11, 12, 13, 14, 17, 19, 20, 21, 22, 25, 26, 28, 30, 31, 32, 33, 35, 38, 39, 41, 43, 44, 45, 46, 48, 50, 56 };
		for (int i : textField) {
			menuCreator->childs[i].ChangeTextByUser(_mousePos, 0, true);
		}

		menu16x9->childs[selectedElement].SetBackground(ToVector2f(menuCreator->childs, 5), ToVector2f(menuCreator->childs, 8), ToColor(menuCreator->childs, 11), ToFloat(menuCreator->childs[17].contentText.getString()), ToColor(menuCreator->childs, 19));
		menu16x9->childs[selectedElement].SetDefaultText(ToVector2f(menuCreator->childs, 25), ToFloat(menuCreator->childs[28].contentText.getString()), ToColor(menuCreator->childs, 30), menuCreator->childs[35].contentText.getString(), *GestionnaireFont::obtenirFont("Pixeled.ttf"));
		menu16x9->childs[selectedElement].SetContentText(ToVector2f(menuCreator->childs, 38), ToFloat(menuCreator->childs[41].contentText.getString()), ToColor(menuCreator->childs, 43), menuCreator->childs[48].contentText.getString(), *GestionnaireFont::obtenirFont("Pixeled.ttf"));

		if (menuCreator->childs[51].isClicked(0, true)) {//Prev
			if (selectedElement != 0) {
				selectedElement--;
				LoadFromATHElement(selectedElement);
				if (selectedElement < 10) {
					menuCreator->childs[52].defaultText.setString("0" + std::to_string(selectedElement));
				}
				else {
					menuCreator->childs[52].defaultText.setString(std::to_string(selectedElement));
				}
			}
		}
		else if (menuCreator->childs[52].isClicked(0, true)) {//# Element	
		}
		else if (menuCreator->childs[53].isClicked(0, true)) {//Next
			if (selectedElement != menu16x9->childs.size() - 1) {
				selectedElement++;
				LoadFromATHElement(selectedElement);
				if (selectedElement < 10) {
					menuCreator->childs[52].defaultText.setString("0" + std::to_string(selectedElement));
				}
				else {
					menuCreator->childs[52].defaultText.setString(std::to_string(selectedElement));
				}
			}
		}
		else if (menuCreator->childs[54].isClicked(0, true)) {//Add
			menu16x9->AddElement();
		}
		else if (menuCreator->childs[57].isClicked(0, true)) {//Save
			menu16x9->Save(menuCreator->childs[56].contentText.getString());
		}
		else if (menuCreator->childs[58].isClicked(0, true)) {//Load
			menu16x9->Load(menuCreator->childs[56].contentText.getString());

			selectedElement = 0;

			if (menu16x9->childs.size() > 0) {
				LoadFromATHElement(selectedElement);
			}
		}

		if (menuCreator->childs[1].isClicked(0, false)) {//Move the canvas
			menu16x9->Move(_deltaMousePos.x * menu_Creator_Origin.scale.x, _deltaMousePos.y * menu_Creator_Origin.scale.y);
		}


		Draw();

		window.setView(camera);
		window.display();
		compteur++;
		if (compteur == 500)
		{

			std::cout << 500 / timerFPS.restart().asSeconds() << " boucles par seconde" << std::endl;
			compteur = 0;
		}
	}

	return 0;
}


void Draw() {
	//window->setView(menuCreator.view);
	globalBackground->Draw();
	menu16x9->Draw();
	menuCreator->Draw();
}

void Resize(unsigned int _newSizeX, unsigned int _newSizeY) {
	menu_Creator_Origin.Scale((float)_newSizeX / menu_Creator_Origin.view->getSize().x, (float)_newSizeY / menu_Creator_Origin.view->getSize().y);

	menu_Creator_Origin.view->setSize(_newSizeX, _newSizeY);

	//le Scale influence les constantes (Offset 300 et 700)
	menuCreator->childs[1].defaultText.setString(std::to_string(_newSizeX - 300) + "x" + std::to_string(_newSizeY));
	menuCreator->childs[2].defaultText.setString(std::to_string(_newSizeX - 700) + "x" + std::to_string(_newSizeY));
}


////////// FLOAt //////////
float ToFloat(std::string _string) {
	return std::stof(_string == "" || _string == "-" ? "0.0" : _string);
}

////////// VECTOR2F //////////
sf::Vector2f ToVector2f(std::vector<ATHElement> _childs, int _firstElement) {
	return sf::Vector2f(ToFloat(_childs[_firstElement].contentText.getString()), ToFloat(_childs[_firstElement + 1].contentText.getString()));
}

////////// COLOR //////////
sf::Color ToColor(std::vector<ATHElement> _childs, int _firstElement) {
	return sf::Color(ToFloat(_childs[_firstElement].contentText.getString()), ToFloat(_childs[_firstElement + 1].contentText.getString()), ToFloat(_childs[_firstElement + 2].contentText.getString()), ToFloat(_childs[_firstElement + 3].contentText.getString()));
}


void LoadFromATHElement(int _child) {
	FillContentText(menu16x9->childs[_child].background.getPosition(), menuCreator->childs, 5);				//Fill content text with Background position
	FillContentText(menu16x9->childs[_child].background.getSize(), menuCreator->childs, 8);					//Fill content text with Background size
	FillContentText(menu16x9->childs[_child].background.getFillColor(), menuCreator->childs, 11);			//Fill content text with Background color

	FillContentText(menu16x9->childs[_child].background.getOutlineThickness(), menuCreator->childs[17]);	//Fill content text with Outline size
	FillContentText(menu16x9->childs[_child].background.getOutlineColor(), menuCreator->childs, 19);		//Fill content text with Outline Color

	FillContentText(menu16x9->childs[_child].defaultText.getPosition(), menuCreator->childs, 25);			//Fill content text with Default Text position
	FillContentText(menu16x9->childs[_child].defaultText.getCharacterSize(), menuCreator->childs[28]);		//Fill content text with Default Text size
	FillContentText(menu16x9->childs[_child].defaultText.getFillColor(), menuCreator->childs, 30);			//Fill content text with Default Text color
	FillContentText(menu16x9->childs[_child].defaultText.getString(), menuCreator->childs[35]);				//Fill content text with Default Text Text

	FillContentText(menu16x9->childs[_child].contentText.getPosition(), menuCreator->childs, 38);			//Fill content text with Content Text position
	FillContentText(menu16x9->childs[_child].contentText.getCharacterSize(), menuCreator->childs[41]);		//Fill content text with Content Text size
	FillContentText(menu16x9->childs[_child].contentText.getFillColor(), menuCreator->childs, 43);			//Fill content text with Content Text color
	FillContentText(menu16x9->childs[_child].contentText.getString(), menuCreator->childs[48]);				//Fill content text with Content Text Text

	menuCreator->childs[50].contentText.setString(menu16x9->childs[_child].name);
}

void FillContentText(sf::Vector2f _vector2f, std::vector<ATHElement>& _tochilds, int _firstElement) {
	_tochilds[_firstElement].contentText.setString(std::to_string(_vector2f.x));
	_tochilds[_firstElement + 1].contentText.setString(std::to_string(_vector2f.y));
}

void FillContentText(sf::Color _color, std::vector<ATHElement>& _tochilds, int _firstElement) {
	_tochilds[_firstElement].contentText.setString(std::to_string(_color.r));
	_tochilds[_firstElement + 1].contentText.setString(std::to_string(_color.g));
	_tochilds[_firstElement + 2].contentText.setString(std::to_string(_color.b));
	_tochilds[_firstElement + 3].contentText.setString(std::to_string(_color.a));
}

void FillContentText(float _float, ATHElement& _toElement) {
	_toElement.contentText.setString(std::to_string(_float));
}

void FillContentText(const sf::Font* _font, ATHElement& _toElement) {
	_toElement.contentText.setString(GestionnaireFont::retrouverNom(_font));
}

void FillContentText(std::string _string, ATHElement& _toElement) {
	_toElement.contentText.setString(_string);
}