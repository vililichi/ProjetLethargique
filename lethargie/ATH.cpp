#include "ATH.h"

///////////////////////////////////////////////ATHElement - Child-Parent System/////////////////////////////////////////////
///////////////////////////Self///////////////////////////

ATHElement::ATHElement(std::string _name, sf::Vector2f _pos, sf::Vector2f _size) :
	parent(), window(), view(), background(), contentText(), defaultText(), name(_name), childs() {
	toggle = false;
	isChangingTextByUser = false;
	prevKey = -1;
	pos = _pos;
}

ATHElement::ATHElement(ATHElement& _parent, std::string _name, sf::Vector2f _pos, sf::Vector2f _size) :
	background(), contentText(), defaultText(), name(_name), childs() {
	parent = &_parent;

	window = parent->window;
	view = parent->view;

	toggle = false;
	isChangingTextByUser = false;
	prevKey = -1;
	pos = _pos;
}

ATHElement::ATHElement(sf::RenderWindow& _window, sf::View& _view, std::string _name, sf::Vector2f _pos, sf::Vector2f _size) :
	parent(), background(), contentText(), defaultText(), name(_name), childs() {
	window = &_window;
	view = &_view;

	toggle = false;
	isChangingTextByUser = false;
	prevKey = -1;
	pos = _pos;
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

void ATHElement::DrawSelf()
{
	if (window != NULL) {
		std::string _text = contentText.getString(), _defaultText = defaultText.getString();
		background.setPosition(background.getPosition() + pos);
		window->draw(background);
		background.setPosition(background.getPosition() - pos);

		if (_text != "") {
			contentText.setPosition(contentText.getPosition() + pos);
			window->draw(contentText);
			contentText.setPosition(contentText.getPosition() + pos);
		}
		else {
			defaultText.setPosition(defaultText.getPosition() + pos);
			window->draw(defaultText);
			defaultText.setPosition(defaultText.getPosition() - pos);
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

void ATHElement::SaveSelf(std::string _path) {
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

void ATHElement::SaveSelf(std::ofstream* _file) {
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

void ATHElement::LoadSelf(std::string _path) {

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

void ATHElement::LoadSelf(std::ifstream* _file) {
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

///////////////////////////Self-Child///////////////////////////


void ATHElement::Draw() {
	window->setView(*view);

	background.setPosition(background.getPosition() + pos);
	defaultText.setPosition(defaultText.getPosition() + pos);
	contentText.setPosition(contentText.getPosition() + pos);

	DrawSelf();

	background.setPosition(background.getPosition() - pos);
	defaultText.setPosition(defaultText.getPosition() - pos);
	contentText.setPosition(contentText.getPosition() - pos);

	for (unsigned int i = 0; i < childs.size(); i++) {
		childs[i].Draw();
	}
}

void ATHElement::Clear() {
	childs.clear();
}

//ATHElement* ATHElement::AddElement() {
//	childs.push_back(ATHElement(*this));
//	return childs.back;
//}

ATHElement* ATHElement::AddElement(ATHElement _element) {
	_element.parent = this;
	_element.view = view;
	_element.window = window;

	childs.push_back(_element);
	return &childs.back();
}

ATHElement* ATHElement::AddElement(std::string _name, sf::Vector2f _pos, sf::Vector2f _size) {
	childs.push_back(ATHElement(*this, _name, _pos, _size));
	return &childs.back();
}

void ATHElement::Load(std::string _path) {
	std::ifstream _file;
	_file.open(_path);

	//Get the number of elements in the file
	const int nbElements = ReadInt(&_file);
	childs.resize(nbElements, ATHElement(*this, ""));

	const sf::Vector2f _scales = ReadVector2f(&_file);
	//view.setSize(ReadVector2f(&_file));

	for (unsigned int i = 0; i < childs.size(); i++) {
		childs[i].LoadSelf(&_file);
	}

	//view.setSize(_scales);

	_file.close();
}

void ATHElement::Save(std::string _path) {
	std::ofstream _file;
	_file.open(_path);

	_file << childs.size() << std::endl;

	for (unsigned int i = 0; i < childs.size(); i++) {
		_file << "----- Element " << i << " : " << childs[i].name << " -----" << std::endl;
		childs[i].SaveSelf(&_file);
		_file << std::endl << std::endl;
	}


	_file.close();
}

ATHElement* ATHElement::FindElement(std::string _name) {
	for (unsigned int i = 0; i < childs.size(); i++) {
		if (childs[i].name == _name) {
			return &childs[i];
		}
	}
	std::cout << "ATHElement ATHElement::FindElement(std::string _name)_" << _name << "_Not_Found" << std::endl;
	return NULL;
}

void ATHElement::SetPosition(sf::Vector2f _newPos) {
	const sf::Vector2f deltaPos = _newPos - pos;
	Move(deltaPos);
}

void ATHElement::SetPosition(float _newPosX, float _newPosY) {
	SetPosition(sf::Vector2f(_newPosX, _newPosY));
}

void ATHElement::Move(sf::Vector2f _move) {
	/*background.setPosition(background.getPosition() + _move);
	defaultText.setPosition(defaultText.getPosition() + _move);
	contentText.setPosition(contentText.getPosition() + _move);*/

	pos += _move;

	for (ATHElement& child : childs) {
		child.Move(_move);
	}
}

void ATHElement::Move(float _moveX, float _moveY) {
	Move(sf::Vector2f(_moveX, _moveY));
}

void ATHElement::Scale(float _scaleX, float _scaleY) {	
	background.scale(_scaleX, _scaleY);
	defaultText.scale(_scaleX, _scaleY);
	contentText.scale(_scaleX, _scaleY);

	background.setPosition(sf::Vector2f(background.getPosition().x * _scaleX, background.getPosition().y * _scaleY));
	defaultText.setPosition(sf::Vector2f(defaultText.getPosition().x * _scaleX, defaultText.getPosition().y * _scaleY));
	contentText.setPosition(sf::Vector2f(contentText.getPosition().x * _scaleX, contentText.getPosition().y * _scaleY));

	for (ATHElement& child : childs) {
		child.Scale(_scaleX, _scaleY);
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
