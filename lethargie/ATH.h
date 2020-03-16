#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>

class ATHManager {
	ATHManager();
};

class Button {
public:
	sf::RenderWindow* window;
	sf::RectangleShape background;
	sf::Color
		backgroundColor,
		outlineColor;

	std::string text, defaultText;

	float posX, posY,
		sizeX, sizeY,
		outlineSize;
	bool toggle;
	Button(sf::RenderWindow& _window);
	Button(sf::RenderWindow& _window, float _posX, float _posY, float _sizeX, float _sizeY, sf::Color _backgroundColor = sf::Color::Transparent, float _outlineSize = 0, sf::Color _outlineColor = sf::Color::Transparent);
	//~Button();

	bool isClicked(int _button, bool _toggle);
	bool isHovered();
	bool isHovered(sf::Vector2f _mousePos);

	void UpdateVar();
	void Draw();
};

