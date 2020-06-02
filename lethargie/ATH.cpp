#include "ATH.h"

///////////////////////////////////////////////Button/////////////////////////////////////////////
ATHElement::ATHElement(std::string _name) : window(), background(), contentText(), defaultText(), name(_name) {
	toggle = false;
	isChangingTextByUser = false;
	prevKey = -1;
}

ATHElement::ATHElement(sf::RenderWindow& _window, std::string _name) : background(), contentText(), defaultText(), name(_name) {
	window = &_window;
	toggle = false;
	isChangingTextByUser = false;
	prevKey = -1;
}

void ATHElement::SetDefaultText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font& _font) {
	defaultText.setPosition(_pos);
	defaultText.setCharacterSize(_size);
	defaultText.setFillColor(_color);
	defaultText.setString(_text);
	defaultText.setFont(_font);
}
void ATHElement::SetContentText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font& _font) {
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

void ATHElement::LoadDefaultText(std::ifstream* _file) {
	defaultText.setPosition(ReadVector2f(_file));
	defaultText.setCharacterSize(ReadInt(_file));
	defaultText.setFillColor(ReadColor(_file));
	defaultText.setString(ReadString(_file, 1));
	defaultText.setFont(*GestionnaireFont::obtenirFont(ReadString(_file, 0)));
}
void ATHElement::LoadContentText(std::ifstream* _file) {
	contentText.setPosition(ReadVector2f(_file));
	contentText.setCharacterSize(ReadInt(_file));
	contentText.setFillColor(ReadColor(_file));
	contentText.setString(ReadString(_file, 1));
	contentText.setFont(*GestionnaireFont::obtenirFont(ReadString(_file, 0)));
}
void ATHElement::LoadBackground(std::ifstream* _file) {
	background.setPosition(ReadVector2f(_file));
	background.setSize(ReadVector2f(_file));
	background.setFillColor(ReadColor(_file));
	background.setOutlineThickness(ReadInt(_file));
	background.setOutlineColor(ReadColor(_file));
}


bool ATHElement::isClicked(int _button = 0, bool _toggle = false) {
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

	if (_mousePos.x > pos.x && _mousePos.x < pos.x + size.x && _mousePos.y > pos.y && _mousePos.y < pos.y + size.y) {
		return true;
	}

	return false;
}

bool ATHElement::isHovered(sf::Vector2i _mousePos) {
	sf::Vector2f pos = background.getPosition(),
		size = background.getSize();

	if (_mousePos.x > pos.x && _mousePos.x < pos.x + size.x && _mousePos.y > pos.y && _mousePos.y < pos.y + size.y) {
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

void ATHElement::ChangeTextByUser(int _mouseButton, bool _toggle, int _sendKey) {

	if (isClicked(_mouseButton, _toggle))
		isChangingTextByUser = true;
	else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)_sendKey)) {
		isChangingTextByUser = false;
	}

	if (isChangingTextByUser) {
		contentText.setString(WriteText());
	}

}

void ATHElement::ChangeTextByUser(sf::Vector2i _mousePos, int _mouseButton, bool _toggle, int _sendKey) {

	if (isClicked(_mousePos, _mouseButton, _toggle))
		isChangingTextByUser = true;
	else if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)_sendKey)) {
		isChangingTextByUser = false;
	}

	if (isChangingTextByUser) {
		contentText.setString(WriteText());
	}

}

std::string ATHElement::WriteText() {


	std::string _text = contentText.getString();
	const std::string keys[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w",
		"x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Escape", "LControl", "", "LAlt", "LSystem", "RControl", "", "RAlt",
		"RSystem", "Menu", "^", "¸", ";", ",", ".", "#", "Slash", "Backslash", "Tilde", "Equal", "-", " ",
		"", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right",
		"Up", "Down", "1", "2", "3", "4", "5", "6", "7", "8", "9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13",
		"F14", "F15", "Pause", "KeyCount", "" };

	const std::string majKeys[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W",
		"X", "Y", "Z", ")", "!", "2", "/", "$", "%", "?", "&", "*", "(", "Escape", "LControl", "", "LAlt", "LSystem", "RControl", "", "RAlt",
		"RSystem", "Menu", "[", "]", ":", "'", ".", "BackSlash", "Slash", "Backslash", "Tilde", "Equal", "_", " ",
		"", "Backspace", "Tab", "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide", "Left", "Right",
		"Up", "Down", "1", "2", "3", "4", "5", "6", "7", "8", "9", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13",
		"F14", "F15", "Pause", "KeyCount", "" };

	int keyPressed;

	for (keyPressed = 0; keyPressed < sf::Keyboard::Key::KeyCount; keyPressed++) {
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyPressed) && keyPressed != sf::Keyboard::LShift && keyPressed != sf::Keyboard::RShift) {
			break;
		}
	}

	//if(keyPressed == sf::Keyboard::Hyphen)

	//Key toggle for writing text
	if (prevKey != keyPressed) {
		if (keyPressed == sf::Keyboard::BackSpace && _text != "") {
			_text.pop_back();
		}
		else if (keyPressed == sf::Keyboard::Delete) {
			_text = "";
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			_text += majKeys[keyPressed];
		}
		else {
			_text += keys[keyPressed];
		}


		contentText.setString(_text);
	}

	prevKey = keyPressed;

	return _text;
}

int GetKeyPressed() {
	int _keyPressed;

	for (_keyPressed = 0; _keyPressed < sf::Keyboard::Key::KeyCount; _keyPressed++) {
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)_keyPressed)) {
			break;
		}
	}

	return _keyPressed;
}

void ATHElement::Save(std::string _path) {
	std::ofstream _file(_path);


	_file
		<< "----- Element Single Save -----" << std::endl
		<< "Background :" << std::endl
		<< ToString(background.getPosition()) << std::endl
		<< ToString(background.getSize()) << std::endl
		<< ToString(background.getFillColor()) << std::endl
		<< background.getOutlineThickness() << std::endl
		<< ToString(background.getOutlineColor()) << std::endl << std::endl

		<< "Default Text :" << std::endl
		<< ToString(defaultText.getPosition()) << std::endl
		<< defaultText.getCharacterSize() << std::endl
		<< ToString(defaultText.getFillColor()) << std::endl
		<< std::string(defaultText.getString()) << std::endl
		<< GestionnaireFont::retrouverNom(defaultText.getFont()) << std::endl << std::endl

		<< "Content Text :" << std::endl
		<< ToString(contentText.getPosition()) << std::endl
		<< contentText.getCharacterSize() << std::endl
		<< ToString(contentText.getFillColor()) << std::endl
		<< std::string(contentText.getString()) << std::endl
		<< GestionnaireFont::retrouverNom(contentText.getFont()) << std::endl << std::endl

		<< "Name :" << std::endl
		<< name << std::endl;

	_file.close();
}

void ATHElement::Save(std::ofstream* _file) {
	*_file
		<< "Background :" << std::endl
		<< ToString(background.getPosition()) << std::endl
		<< ToString(background.getSize()) << std::endl
		<< ToString(background.getFillColor()) << std::endl
		<< background.getOutlineThickness() << std::endl
		<< ToString(background.getOutlineColor()) << std::endl << std::endl

		<< "Default Text :" << std::endl
		<< ToString(defaultText.getPosition()) << std::endl
		<< defaultText.getCharacterSize() << std::endl
		<< ToString(defaultText.getFillColor()) << std::endl
		<< std::string(defaultText.getString()) << std::endl
		<< GestionnaireFont::retrouverNom(defaultText.getFont()) << std::endl << std::endl

		<< "Content Text :" << std::endl
		<< ToString(contentText.getPosition()) << std::endl
		<< contentText.getCharacterSize() << std::endl
		<< ToString(contentText.getFillColor()) << std::endl
		<< std::string(contentText.getString()) << std::endl
		<< GestionnaireFont::retrouverNom(contentText.getFont()) << std::endl << std::endl

		<< "Name :" << std::endl
		<< name << std::endl;
}

void ATHElement::Load(std::string _path) {

	std::ifstream _file;
	_file.open(_path);

	ReadSkipTitle(&_file); //Skip Element #

	//Background
	ReadSkipTitle(&_file); //Skip Background :
	LoadBackground(&_file);

	//DefaultText
	ReadString(&_file, 2); //Skip Default text :
	LoadDefaultText(&_file);

	//ContentText
	ReadString(&_file, 1); //Skip Content Text:
	LoadContentText(&_file);

	ReadString(&_file, 1); //Skip name:
	std::string _name = ReadString(&_file, 0);
	name = _name;

	_file.close();
}

void ATHElement::Load(std::ifstream* _file) {
	ReadSkipTitle(_file);	//Skip Element #

	//Background
	ReadSkipTitle(_file);	//Skip Background :
	LoadBackground(_file);

	//DefaultText
	ReadSkipTitle(_file);	//Skip Default text :
	LoadDefaultText(_file);

	//ContentText
	ReadSkipTitle(_file);	//Skip Content Text:
	LoadContentText(_file);

	ReadSkipTitle(_file);	//Skip name:
	std::string _name = ReadString(_file, 0);
	name = _name;
}

////////////////////////////////////////////////////////////////////////ATH Manager/////////////////////////////////////////////

ATHManager::ATHManager() : window() {}

ATHManager::ATHManager(sf::RenderWindow& _window, sf::Vector2f _pos, sf::Vector2f _size) :
	window(&_window), view(_pos, _size) {}

void ATHManager::Draw() {
	window->setView(view);
	for (int i = 0; i < elements.size(); i++) {
		elements[i].Draw();
	}
}

void ATHManager::Clear() {
	elements.clear();
}

void ATHManager::AddElement() {
	elements.push_back(ATHElement(*window));
}

void ATHManager::AddElement(ATHElement _element) {
	_element.window = window;
	elements.push_back(_element);
}

void ATHManager::Load(std::string _path) {
	std::ifstream _file;
	_file.open(_path);

	//Get the number of elements in the file
	const int nbElements = ReadInt(&_file);
	elements.resize(nbElements, ATHElement(*window));

	const sf::Vector2f _scales = ReadVector2f(&_file);
	//view.setSize(ReadVector2f(&_file));

	for (int i = 0; i < elements.size(); i++) {
		elements[i].Load(&_file);
	}

	//view.setSize(_scales);

	_file.close();
}

void ATHManager::Save(std::string _path) {
	std::ofstream _file;
	_file.open(_path);

	_file << elements.size() << std::endl;

	for (int i = 0; i < elements.size(); i++) {
		_file << "----- Element " << i << " : " << elements[i].name << " -----" << std::endl;
		elements[i].Save(&_file);
		_file << std::endl << std::endl;
	}


	_file.close();
}

ATHElement* ATHManager::FindElement(std::string _name) {
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i].name == _name) {
			return &elements[i];
		}
	}
	std::cout << "ATHElement ATHManager::FindElement(std::string _name)_" << _name << "_Not_Found" << std::endl;
	return NULL;
}

void ATHManager::SetPosition(sf::Vector2f _pos) {
	view.setCenter(_pos);
}
void ATHManager::SetPosition(int _posX, int _posY) {
	view.setCenter(_posX, _posY);
}

void ATHManager::Move(sf::Vector2f _move) {
	view.move(_move);
}
void ATHManager::Move(int _moveX, int _moveY) {
	view.move(_moveX, -_moveY);
}

void ATHManager::Resize(sf::Vector2f _newSize) {
	sf::Vector2f _scale(sf::Vector2f(_newSize.x / view.getSize().x, _newSize.y / view.getSize().y));
	view.setSize(_newSize.x, _newSize.y);

	for (ATHElement& element : elements) {
		element.background.scale(_scale);
		element.defaultText.scale(_scale);
		element.contentText.scale(_scale);

		element.background.setPosition(sf::Vector2f(element.background.getPosition().x * _scale.x, element.background.getPosition().y * _scale.y));
		element.defaultText.setPosition(sf::Vector2f(element.defaultText.getPosition().x * _scale.x, element.defaultText.getPosition().y * _scale.y));
		element.contentText.setPosition(sf::Vector2f(element.contentText.getPosition().x * _scale.x, element.contentText.getPosition().y * _scale.y));
	}
}

void ATHManager::Resize(unsigned int _newSizeX, unsigned int _newsizeY) {
	sf::Vector2f _scale(sf::Vector2f(_newSizeX / view.getSize().x, _newsizeY / view.getSize().y));
	view.setSize(_newSizeX, _newsizeY);

	for (ATHElement& element : elements) {
		element.background.scale(_scale);
		element.defaultText.scale(_scale);
		element.contentText.scale(_scale);

		element.background.setPosition(sf::Vector2f(element.background.getPosition().x * _scale.x, element.background.getPosition().y * _scale.y));
		element.defaultText.setPosition(sf::Vector2f(element.defaultText.getPosition().x * _scale.x, element.defaultText.getPosition().y * _scale.y));
		element.contentText.setPosition(sf::Vector2f(element.contentText.getPosition().x * _scale.x, element.contentText.getPosition().y * _scale.y));
	}
}

void ATHManager::Scale(float _scale) {
	const sf::Vector2f _scale2f = sf::Vector2f(_scale, _scale);
	//view.zoom(_scale);

	for (ATHElement& element : elements) {
		element.background.scale(_scale2f);
		element.defaultText.scale(_scale2f);
		element.contentText.scale(_scale2f);

		element.background.setPosition(sf::Vector2f(element.background.getPosition().x * _scale, element.background.getPosition().y * _scale));
		element.defaultText.setPosition(sf::Vector2f(element.defaultText.getPosition().x * _scale, element.defaultText.getPosition().y * _scale));
		element.contentText.setPosition(sf::Vector2f(element.contentText.getPosition().x * _scale, element.contentText.getPosition().y * _scale));
	}
}

////////////////////////////////////////////////////////Global Function ///////////////////////////////////////////

std::string ToString(sf::Color _color, char _insertChar) {
	return std::to_string(_color.r) + _insertChar + std::to_string(_color.g) + _insertChar + std::to_string(_color.b) + _insertChar + std::to_string(_color.a);
}

std::string ToString(sf::Vector2f _vector, char _insertChar) {
	return std::to_string(_vector.x) + _insertChar + std::to_string(_vector.y);
}

sf::Vector2f ReadVector2f(std::ifstream* _file) {

	sf::Vector2f _vector2f;
	*_file >> _vector2f.x >> _vector2f.y;
	return _vector2f;
}

sf::Color ReadColor(std::ifstream* _file) {

	int _r, _g, _b, _a;
	*_file >> _r >> _g >> _b >> _a;
	return sf::Color(_r, _g, _b, _a);
}

std::string ReadString(std::ifstream* _file, int _lineSkip) {

	std::string _string = "";

	for (int i = 0; i < _lineSkip; i++)
		std::getline(*_file, _string);
	std::getline(*_file, _string);

	return _string;
}


int ReadInt(std::ifstream* _file) {

	int _int;
	*_file >> _int;
	return _int;
}

void ReadSkipTitle(std::ifstream* _file) {
	std::string _string;
	*_file >> _string;
	ReadString(_file);
}
