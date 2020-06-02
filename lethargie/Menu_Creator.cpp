#include "Menu_Creator.h"
void Menu_Creator::Start(sf::RenderWindow* _window) {
	window = _window;

	//hud.setCenter(0, 0);
	//hud.setSize(1900, 900);
	////hud.setCenter(hud.getSize() * 0.5f);

	//cam16x9.setCenter(150, 0);
	//cam16x9.setSize(1600, 900);
	globalBackground = ATHElement(*window);
	globalBackground.SetBackground(sf::Vector2f(-950, -450), sf::Vector2f(1900, 900), sf::Color(20, 0, 20, 255), 0, sf::Color::Transparent);

	menuCreator = ATHManager(*window, sf::Vector2f(0, 0), sf::Vector2f(1900, 900));
	//menu16x9 = ATHManager(*window, sf::Vector2f(150, 0), sf::Vector2f(1600, 900));
	menu16x9 = ATHManager(*window, sf::Vector2f(0, 0), sf::Vector2f(1900, 900));

	menuCreator.Load("Menu_Creator.txt");
	menu16x9.AddElement();

	selectedElement = 0;
	//menuCreator.Save("Menu_Creator.txt");
}

void Menu_Creator::Update() {
	_oldMousePos = _mousePos;
	_mousePos = sf::Mouse::getPosition(*window) - (sf::Vector2i)window->getSize() / 2;
	_deltaMousePos = _mousePos - _oldMousePos;
	const int textField[] = { 5, 6, 8, 9, 11, 12, 13, 14, 17, 19, 20, 21, 22, 25, 26, 28, 30, 31, 32, 33, 35, 38, 39, 41, 43, 44, 45, 46, 48, 50, 56 };
	for (int i : textField) {
		menuCreator.elements[i].ChangeTextByUser(_mousePos, 0, true);
	}

	menu16x9.elements[selectedElement].SetBackground(ToVector2f(menuCreator.elements, 5), ToVector2f(menuCreator.elements, 8), ToColor(menuCreator.elements, 11), ToFloat(menuCreator.elements[17].contentText.getString()), ToColor(menuCreator.elements, 19));
	menu16x9.elements[selectedElement].SetDefaultText(ToVector2f(menuCreator.elements, 25), ToFloat(menuCreator.elements[28].contentText.getString()), ToColor(menuCreator.elements, 30), menuCreator.elements[35].contentText.getString(), *GestionnaireFont::obtenirFont("Pixeled.ttf"));
	menu16x9.elements[selectedElement].SetContentText(ToVector2f(menuCreator.elements, 38), ToFloat(menuCreator.elements[41].contentText.getString()), ToColor(menuCreator.elements, 43), menuCreator.elements[48].contentText.getString(), *GestionnaireFont::obtenirFont("Pixeled.ttf"));

	if (menuCreator.elements[51].isClicked(0, true)) {//Prev
		if (selectedElement != 0) {
			selectedElement--;
			LoadFromATHElement(selectedElement);
			if (selectedElement < 10) {
				menuCreator.elements[52].defaultText.setString("0" + std::to_string(selectedElement));
			}
			else {
				menuCreator.elements[52].defaultText.setString(std::to_string(selectedElement));
			}
		}
	}
	else if (menuCreator.elements[52].isClicked(0, true)) {//# Element	
	}
	else if (menuCreator.elements[53].isClicked(0, true)) {//Next
		if (selectedElement + 1 < menu16x9.elements.size()) {
			selectedElement++;
			LoadFromATHElement(selectedElement);
			if (selectedElement < 10) {
				menuCreator.elements[52].defaultText.setString("0" + std::to_string(selectedElement));
			}
			else {
				menuCreator.elements[52].defaultText.setString(std::to_string(selectedElement));
			}
		}
	}
	else if (menuCreator.elements[54].isClicked(0, true)) {//Add
		menu16x9.AddElement();
	}
	else if (menuCreator.elements[57].isClicked(0, true)) {//Save
		menu16x9.Save(menuCreator.elements[56].contentText.getString());
	}
	else if (menuCreator.elements[58].isClicked(0, true)) {//Load
		menu16x9.Load(menuCreator.elements[56].contentText.getString());

		selectedElement = 0;

		if (menu16x9.elements.size() > 0) {
			LoadFromATHElement(selectedElement);
		}
	}

	if (menuCreator.elements[1].isClicked(0, false)) {//Move the canvas
		menu16x9.view.move(-_deltaMousePos.x, -_deltaMousePos.y);
	}
}

void Menu_Creator::Draw() {
	window->setView(menuCreator.view);
	globalBackground.Draw();
	menu16x9.Draw();
	menuCreator.Draw();
}

void Menu_Creator::Resize(sf::Vector2f _newSize) {
	menuCreator.Resize(_newSize);

	menuCreator.elements[1].defaultText.setString(std::to_string(_newSize.x - 300) + "x" + std::to_string(_newSize.y));
	menuCreator.elements[2].defaultText.setString(std::to_string(_newSize.x - 700) + "x" + std::to_string(_newSize.y));
}

void Menu_Creator::Resize(unsigned int _newSizeX, unsigned int _newSizeY) {
	menuCreator.Resize(_newSizeX, _newSizeY);

	menuCreator.elements[1].defaultText.setString(std::to_string(_newSizeX - 300) + "x" + std::to_string(_newSizeY));
	menuCreator.elements[2].defaultText.setString(std::to_string(_newSizeX - 700) + "x" + std::to_string(_newSizeY));
}

void Menu_Creator::Scale(unsigned int _scale) {
	menuCreator.Scale(_scale);

	menuCreator.elements[1].defaultText.setString(std::to_string(menuCreator.view.getSize().x - 300) + "x" + std::to_string(menuCreator.view.getSize().y));
	menuCreator.elements[2].defaultText.setString(std::to_string(menuCreator.view.getSize().x - 700) + "x" + std::to_string(menuCreator.view.getSize().y));
}


void Menu_Creator::mouseWheel(int _mouseWheelDelta) {//Zoom in Zoom out Canvas
	menu16x9.view.zoom(1 + _mouseWheelDelta * 0.05f);
}


////////// FLOAt //////////
float ToFloat(std::string _string) {
	return std::stof(_string == "" || _string == "-" ? "0.0" : _string);
}

////////// VECTOR2F //////////
sf::Vector2f ToVector2f(std::vector<ATHElement> _elements, int _firstElement) {
	return sf::Vector2f(ToFloat(_elements[_firstElement].contentText.getString()), ToFloat(_elements[_firstElement + 1].contentText.getString()));
}

////////// COLOR //////////
sf::Color ToColor(std::vector<ATHElement> _elements, int _firstElement) {
	return sf::Color(ToFloat(_elements[_firstElement].contentText.getString()), ToFloat(_elements[_firstElement + 1].contentText.getString()), ToFloat(_elements[_firstElement + 2].contentText.getString()), ToFloat(_elements[_firstElement + 3].contentText.getString()));
}


void Menu_Creator::LoadFromATHElement(int _element) {
	FillContentText(menu16x9.elements[_element].background.getPosition(), menuCreator.elements, 5);				//Fill content text with Background position
	FillContentText(menu16x9.elements[_element].background.getSize(), menuCreator.elements, 8);					//Fill content text with Background size
	FillContentText(menu16x9.elements[_element].background.getFillColor(), menuCreator.elements, 11);			//Fill content text with Background color

	FillContentText(menu16x9.elements[_element].background.getOutlineThickness(), menuCreator.elements[17]);	//Fill content text with Outline size
	FillContentText(menu16x9.elements[_element].background.getOutlineColor(), menuCreator.elements, 19);		//Fill content text with Outline Color

	FillContentText(menu16x9.elements[_element].defaultText.getPosition(), menuCreator.elements, 25);			//Fill content text with Default Text position
	FillContentText(menu16x9.elements[_element].defaultText.getCharacterSize(), menuCreator.elements[28]);		//Fill content text with Default Text size
	FillContentText(menu16x9.elements[_element].defaultText.getFillColor(), menuCreator.elements, 30);			//Fill content text with Default Text color
	FillContentText(menu16x9.elements[_element].defaultText.getString(), menuCreator.elements[35]);				//Fill content text with Default Text Text

	FillContentText(menu16x9.elements[_element].contentText.getPosition(), menuCreator.elements, 38);			//Fill content text with Content Text position
	FillContentText(menu16x9.elements[_element].contentText.getCharacterSize(), menuCreator.elements[41]);		//Fill content text with Content Text size
	FillContentText(menu16x9.elements[_element].contentText.getFillColor(), menuCreator.elements, 43);			//Fill content text with Content Text color
	FillContentText(menu16x9.elements[_element].contentText.getString(), menuCreator.elements[48]);				//Fill content text with Content Text Text

	menuCreator.elements[50].contentText.setString(menu16x9.elements[_element].name);
}

void FillContentText(sf::Vector2f _vector2f, std::vector<ATHElement> &_toElements, int _firstElement) {
	_toElements[_firstElement].contentText.setString(std::to_string(_vector2f.x));
	_toElements[_firstElement + 1].contentText.setString(std::to_string(_vector2f.y));
}

void FillContentText(sf::Color _color, std::vector<ATHElement> &_toElements, int _firstElement) {
	_toElements[_firstElement].contentText.setString(std::to_string(_color.r));
	_toElements[_firstElement + 1].contentText.setString(std::to_string(_color.g));
	_toElements[_firstElement + 2].contentText.setString(std::to_string(_color.b));
	_toElements[_firstElement + 3].contentText.setString(std::to_string(_color.a));
}

void FillContentText(float _float, ATHElement& _toElement) {
	_toElement.contentText.setString(std::to_string(_float));
}

void FillContentText(const sf::Font *_font, ATHElement& _toElement) {
	_toElement.contentText.setString(GestionnaireFont::retrouverNom(_font));
}

void FillContentText(std::string _string, ATHElement& _toElement) {
	_toElement.contentText.setString(_string);
}
