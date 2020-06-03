#pragma once
#include "ATH.h"

struct Menu_Creator {
	sf::RenderWindow* window;

	sf::Vector2i _deltaMousePos, _oldMousePos,_mousePos;

	ATHElement globalBackground;
	ATHElement menuCreator;
	ATHElement menu16x9;

	int selectedElement;

	void Start(sf::RenderWindow* _window);
	void Update();
	void Draw();
	void LoadFromATHElement(int _element);

	
	//void Resize(sf::Vector2f _newSize);
	void Resize(unsigned int _newSizeX, unsigned int _newSizeY);
	//void Scale(float _scaleX, float _scaleY);

	void mouseWheel(int _mouseWheelDelta);
};

float ToFloat(std::string _string);

sf::Vector2f ToVector2f(std::vector<ATHElement> _elements, int _firstElement);

sf::Color ToColor(std::vector<ATHElement> _elements, int _firstElement);

void FillContentText(sf::Vector2f _vector2f, std::vector<ATHElement>& _toElements, int _firstElement);

void FillContentText(sf::Color _color, std::vector<ATHElement>& _toElements, int _firstElement);

void FillContentText(float _float, ATHElement& _toElement);

void FillContentText(const sf::Font *_font, ATHElement& _toElement);

void FillContentText(std::string _string, ATHElement& _toElement);