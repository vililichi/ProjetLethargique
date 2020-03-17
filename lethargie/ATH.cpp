#include "ATH.h"

///////////////////////////////////////////////Button/////////////////////////////////////////////
ATHElement::ATHElement() : window(), background(), contentText(), defaultText() {
	toggle = false;
	prevKey = -1;
}

ATHElement::ATHElement(sf::RenderWindow& _window) : background(), contentText(), defaultText() {
	window = &_window;
	toggle = false;
	prevKey = -1;
}

void ATHElement::SetDefaultText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font &_font) {
	defaultText.setPosition(_pos);
	defaultText.setCharacterSize(_size);
	defaultText.setFillColor(_color);
	defaultText.setString(_text);
	defaultText.setFont(_font);
}
void ATHElement::SetContentText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font &_font) {
	contentText.setPosition(_pos);
	contentText.setCharacterSize(_size);
	contentText.setFillColor(_color);
	contentText.setString(_text);
	contentText.setFont(_font);
}
void ATHElement::SetBackground(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _color, float _outlineSize, sf::Color _outlineColor) {
	background.setPosition(_pos);
	background.setSize(_size);
	background.setFillColor(_color);
	background.setOutlineThickness(_outlineSize);
	background.setOutlineColor(_outlineColor);
}


bool ATHElement::isClicked(int _ATHElement = 0, bool _toggle = false) {
	if (isHovered() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
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

bool ATHElement::isClicked(sf::Vector2i _mousePos, int _button, bool _toggle) {

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

bool ATHElement::isHovered() {
	sf::Vector2i _mousePos = sf::Mouse::getPosition(*window) - (sf::Vector2i)window->getSize() / 2;

	sf::Vector2f pos = background.getPosition(), size = background.getSize();

	if (_mousePos.x > pos.x&& _mousePos.x < pos.x + size.x && _mousePos.y > pos.y&& _mousePos.y < pos.y + size.y) {
		return true;
	}

	return false;
}

bool ATHElement::isHovered(sf::Vector2i _mousePos) {
	sf::Vector2f pos = background.getPosition(),
		size = background.getSize();

	if (_mousePos.x > pos.x&& _mousePos.x < pos.x + size.x && _mousePos.y > pos.y&& _mousePos.y < pos.y + size.y) {
		return true;
	}

	return false;
}

void ATHElement::Draw()
{
	if (window != NULL) {
		std::string _text = contentText.getString(), _defaultText = defaultText.getString();
		window->draw(background);
		if (_text != "") {
			window->draw(contentText);
		}
		else {
			window->draw(defaultText);
		}
	}
	else {
		std::cout << "void ATHElement::Draw_ERROR_window_is_Null" << std::endl;
	}
}

std::string ATHElement::changeTextByUser(sf::Keyboard::Key sendKey, enum sf::Keyboard::Key backKey, sf::View& hud) {

	std::string _text = contentText.getString();
	const std::string keys[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w",
		"x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt",
		"RSystem", "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period", "Quote", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", " ",
		"Enter", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right",
		"Up", "Down", "1", "2", "3", "4", "5", "6", "7", "8", "9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13",
		"F14", "F15", "Pause", "KeyCount", "Dash = Hyphen" };

	const std::string majKeys[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W",
		"X", "Y", "Z", ")", "!", "2", "/", "$", "%", "?", "&", "*", "(", "Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt",
		"RSystem", "Menu", "LBracket", "RBracket", "Semicolon", "Comma", "Period", "Quote", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", " ",
		"Enter", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right",
		"Up", "Down", "1", "2", "3", "4", "5", "6", "7", "8", "9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13",
		"F14", "F15", "Pause", "KeyCount", "Dash = Hyphen" };

	while (!sf::Keyboard::isKeyPressed(sendKey)) {
		int keyPressed;

		for (keyPressed = 0; keyPressed < sf::Keyboard::Key::KeyCount; keyPressed++) {
			if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyPressed)) {
				break;
			}
		}

		//Key toggle for writing text
		if (prevKey != keyPressed) {
			if (keyPressed <= 35 || keyPressed == 57)
			{
				if (prevKey == 42 || prevKey == 38)
					_text += majKeys[keyPressed];
				else
					_text += keys[keyPressed];
			}
			else if (keyPressed == 59 && _text != "") {
				_text.pop_back();
			}
			else if (keyPressed == 66) {
				_text = "";
			}
		}

		prevKey = keyPressed;
		contentText.setString(_text);

		//update corp noControl
		//update ATH
		//update Graphics

		window->setView(hud);
		Draw();

		window->display();
	}

	return _text;
}

//ATHManager::ATHManager() {
//
//}
//
//void ATHManager::Draw() {
//	for (int i = 0; i < nbButtons; i++) {
//		buttons[i].Draw();
//	}
//}