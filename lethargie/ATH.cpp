#include "ATH.h"

///////////////////////////////////////////////Button/////////////////////////////////////////////
Button::Button(sf::RenderWindow& _window) {
	window = &_window;
	posX = 0;
	posY = 0;
	sizeX = 0;
	sizeY = 0;
	backgroundColor = sf::Color::Transparent;
	outlineSize = 0;
	outlineColor = sf::Color::Transparent;
	toggle = false;
	UpdateVar();
}

Button::Button(sf::RenderWindow& _window, float _posX, float _posY, float _sizeX, float _sizeY, sf::Color _backgroundColor, float _outlineSize, sf::Color _outlineColor) {
	window = &_window;
	posX = _posX;
	posY = _posY;
	sizeX = _sizeX;
	sizeY = _sizeY;
	backgroundColor = _backgroundColor;
	outlineSize = _outlineSize;
	outlineColor = _outlineColor; 
	toggle = false;
	UpdateVar();
}

bool Button::isClicked(int _button = 0, bool _toggle = false) {
	sf::Vector2f _mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if (isHovered(_mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (_toggle)
		{
			if (!toggle) {
				toggle = true;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}
	else {
		toggle = false;
	}

	return false;
}

bool Button::isHovered() {
	sf::Vector2f _mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if (_mousePos.x > posX && _mousePos.x < posX + sizeX && _mousePos.y > posY && _mousePos.y < posY + sizeY) {
		return true;
	}

	return false;
}

bool Button::isHovered(sf::Vector2f _mousePos) {
	if (_mousePos.x > posX&& _mousePos.x < posX + sizeX && _mousePos.y > posY&& _mousePos.y < posY + sizeY) {
		return true;
	}

	return false;
}

void Button::UpdateVar() {
	background.setPosition(sf::Vector2f(posX, posY));
	background.setSize(sf::Vector2f(sizeX, sizeY));
	background.setFillColor(backgroundColor);
	background.setOutlineThickness(outlineSize);
	background.setOutlineColor(outlineColor);
}

void Button::Draw()
{
	window->draw(background);
}