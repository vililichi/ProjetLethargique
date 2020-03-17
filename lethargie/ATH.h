#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>

class ATHElement {
public:
	sf::RenderWindow* window;
	sf::RectangleShape background;
	sf::Text contentText, defaultText;

	int prevKey;

	bool toggle;
	ATHElement();
	ATHElement(sf::RenderWindow& _window);
	
	void SetDefaultText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font &_font);
	void SetContentText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font &_font);
	void SetBackground(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _color, float _outlineSize, sf::Color _outlineColor);

	bool isClicked(int _button, bool _toggle);
	bool isClicked(sf::Vector2i _mousePos, int _button = 0, bool _toggle = false);
	bool isHovered();
	bool isHovered(sf::Vector2i _mousePos);

	std::string changeTextByUser(sf::Keyboard::Key sendKey, sf::Keyboard::Key backKey, sf::View& hud);

	void Draw();
};

//class ATHManager {
//	sf::RenderWindow* window;
//	Button buttons[];
//	int nbButtons;
//
//	ATHManager();
//
//	void Draw();
//};
