#include "ATH.h"

using namespace ATH;

///////////////////////////////////////////////ATHElement - Child-Parent System/////////////////////////////////////////////
#pragma region Constructor
	#pragma region Empty - Cant be draw cause no windows and view
		ATHElement::ATHElement(std::string _name, sf::Vector2f _pos, sf::Vector2f _scale) :
		parent(), window(), view(), background(), contentText(), defaultText(), name(_name), childs() {
		pos = _pos;
		scale = _scale;

		toggle = false;
		isChangingTextByUser = false;
		prevKey = -1;
	}
	#pragma endregion

	#pragma region Child - Cant work without a parent who have a window and a view 
		ATHElement::ATHElement(ATHElement& _parent, std::string _name, sf::Vector2f _pos) :
			background(), contentText(), defaultText(), name(_name), childs() {
			parent = &_parent;

			window = parent->window;
			view = parent->view;

			pos = parent->pos;
			scale = parent->scale;

			toggle = false;
			isChangingTextByUser = false;
			prevKey = -1;
		}

		ATHElement::ATHElement(ATHElement& _parent, std::string _name) :
			background(), contentText(), defaultText(), name(_name), childs() {
			parent = &_parent;

			window = parent->window;
			view = parent->view;

			pos = parent->pos;
			scale = parent->scale;

			toggle = false;
			isChangingTextByUser = false;
			prevKey = -1;
		}
	#pragma endregion

	#pragma region Parent - Work alone
		ATHElement::ATHElement(sf::RenderWindow& _window, sf::View& _view, std::string _name, sf::Vector2f _pos, sf::Vector2f _scale) :
			parent(), background(), contentText(), defaultText(), name(_name), childs(), scale(_scale) {
			window = &_window;
			view = &_view;

			toggle = false;
			isChangingTextByUser = false;
			prevKey = -1;
			pos = _pos;
		}
	#pragma endregion
#pragma endregion

#pragma region Self function
	#pragma region Visual
		void ATHElement::SetDefaultText(sf::Vector2f _pos, unsigned int _size, sf::Color _color, std::string _text, sf::Font& _font) {
			defaultText.setPosition(_pos);
			defaultText.setCharacterSize(_size);
			defaultText.setFillColor(_color);
			defaultText.setString(_text);
			defaultText.setFont(_font);
		}
		void ATHElement::SetContentText(sf::Vector2f _pos, unsigned int _size, sf::Color _color, std::string _text, sf::Font& _font) {
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

		void ATHElement::DrawSelf()
		{
			if (window != NULL) {
				std::string _text = contentText.getString(), _defaultText = defaultText.getString();

				//Draw Background
				sf::Vector2f oldBackgroundPos = background.getPosition();

				background.setPosition(oldBackgroundPos.x * background.getScale().x * scale.x + pos.x, oldBackgroundPos.y * background.getScale().y * scale.y + pos.y);
				background.setScale(scale);

				window->draw(background);

				background.setScale(1, 1);
				background.setPosition(oldBackgroundPos);


				if (_text != "") {
					//Draw ContentText
					sf::Vector2f oldContentTextPos(contentText.getPosition());

					contentText.setPosition(oldContentTextPos.x * contentText.getScale().x * scale.x + pos.x, oldContentTextPos.y * contentText.getScale().y * scale.y + pos.y);
					contentText.setScale(scale);

					window->draw(contentText);

					contentText.setScale(1, 1);
					contentText.setPosition(oldContentTextPos);
				}
				else {
					//Draw DefaultText
					sf::Vector2f oldDefaultTextPos(defaultText.getPosition());

					defaultText.setPosition(oldDefaultTextPos.x * defaultText.getScale().x * scale.x + pos.x, oldDefaultTextPos.y * defaultText.getScale().y * scale.y + pos.y);
					defaultText.setScale(scale);

					window->draw(defaultText);

					defaultText.setScale(1, 1);
					defaultText.setPosition(oldDefaultTextPos);
				}
			}
			else {
				std::cout << "void ATHElement::Draw_ERROR_window_is_Null" << std::endl;
			}
		}
	#pragma endregion
	#pragma region Elements Actions
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

			/*sf::Vector2f _pos = background.getPosition(), size = background.getSize();

			if (_mousePos.x > _pos.x && _mousePos.x < _pos.x + size.x && _mousePos.y > _pos.y && _mousePos.y < _pos.y + size.y) {
				return true;
			}*/

			return isHovered(_mousePos);;
		}
		bool ATHElement::isHovered(sf::Vector2i _mousePos) {
			sf::Vector2f _pos = background.getPosition(),
				_size = background.getSize();


			if (_mousePos.x > _pos.x * scale.x && _mousePos.x < (_pos.x + _size.x) * scale.x && _mousePos.y > _pos.y * scale.y && _mousePos.y < (_pos.y + _size.y) * scale.y) {
				return true;
			}

			return false;
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
	#pragma endregion
	#pragma region Files
		void ATHElement::LoadDefaultText(std::ifstream* _file, unsigned int _numTab) {
			defaultText.setPosition(ReadVector2f(_file));
			defaultText.setCharacterSize(ReadInt(_file));
			defaultText.setFillColor(ReadColor(_file));
			defaultText.setString(ReadString(_file, 1, _numTab));
			defaultText.setFont(*GestionnaireFont::obtenirFont(ReadString(_file, 0, _numTab)));
		}
		void ATHElement::LoadContentText(std::ifstream* _file, unsigned int _numTab) {
			contentText.setPosition(ReadVector2f(_file));
			contentText.setCharacterSize(ReadInt(_file));
			contentText.setFillColor(ReadColor(_file));
			contentText.setString(ReadString(_file, 1, _numTab));
			contentText.setFont(*GestionnaireFont::obtenirFont(ReadString(_file, 0, _numTab)));
		}
		void ATHElement::LoadBackground(std::ifstream* _file) {
			background.setPosition(ReadVector2f(_file));
			background.setSize(ReadVector2f(_file));
			background.setFillColor(ReadColor(_file));
			background.setOutlineThickness(ReadInt(_file));
			background.setOutlineColor(ReadColor(_file));
		}

		/*void ATHElement::SaveSelf(std::string _path) {
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
		}*/
		void ATHElement::SaveSelf(std::ofstream* _file, unsigned int _numTab) {
			*_file
				<< name << std::endl

				<< tab(_numTab) << "Childs Size :" << std::endl
				<< tab(_numTab) << childs.size() << std::endl << std::endl

				<< tab(_numTab) << "Background :" << std::endl
				<< tab(_numTab) << ToString(background.getPosition()) << std::endl
				<< tab(_numTab) << ToString(background.getSize()) << std::endl
				<< tab(_numTab) << ToString(background.getFillColor()) << std::endl
				<< tab(_numTab) << background.getOutlineThickness() << std::endl
				<< tab(_numTab) << ToString(background.getOutlineColor()) << std::endl << std::endl

				<< tab(_numTab) << "Default Text :" << std::endl
				<< tab(_numTab) << ToString(defaultText.getPosition()) << std::endl
				<< tab(_numTab) << defaultText.getCharacterSize() << std::endl
				<< tab(_numTab) << ToString(defaultText.getFillColor()) << std::endl
				<< tab(_numTab) << std::string(defaultText.getString()) << std::endl
				<< tab(_numTab) << GestionnaireFont::retrouverNom(defaultText.getFont()) << std::endl << std::endl

				<< tab(_numTab) << "Content Text :" << std::endl
				<< tab(_numTab) << ToString(contentText.getPosition()) << std::endl
				<< tab(_numTab) << contentText.getCharacterSize() << std::endl
				<< tab(_numTab) << ToString(contentText.getFillColor()) << std::endl
				<< tab(_numTab) << std::string(contentText.getString()) << std::endl
				<< tab(_numTab) << GestionnaireFont::retrouverNom(contentText.getFont()) << std::endl << std::endl

				<< tab(_numTab) << "Name :" << std::endl
				<< tab(_numTab) << name << std::endl;
		}

		//void ATHElement::LoadSelf(std::string _path) {

		//	std::ifstream _file;
		//	_file.open(_path);

		//	ReadSkipTitle(&_file); //Skip Element #

		//	//Background
		//	ReadSkipTitle(&_file); //Skip Background :
		//	LoadBackground(&_file);

		//	//DefaultText
		//	ReadString(&_file, 2); //Skip Default text :
		//	LoadDefaultText(&_file);

		//	//ContentText
		//	ReadString(&_file, 1); //Skip Content Text:
		//	LoadContentText(&_file);

		//	ReadString(&_file, 1); //Skip name:
		//	std::string _name = ReadString(&_file, 0);
		//	name = _name;

		//	_file.close();
		//}
		void ATHElement::LoadSelf(std::ifstream* _file, unsigned int _numTab) {
			ReadSkipTitle(_file);	//Skip Childs Reference

			//Get the childs_size s in the file
			ReadSkipTitle(_file);	//Skip Childs Size :
			const int childsSize = ReadInt(_file);
			Clear();
			AddElement("", childsSize);

			//Background
			ReadSkipTitle(_file);	//Skip Background :
			LoadBackground(_file);

			//DefaultText
			ReadSkipTitle(_file);	//Skip Default text :
			LoadDefaultText(_file, _numTab);

			//ContentText
			ReadSkipTitle(_file);	//Skip Content Text:
			LoadContentText(_file, _numTab);

			ReadSkipTitle(_file);	//Skip name:
			std::string _name = ReadString(_file, 0);
			name = _name;
		}		
		void ATHElement::OldLoadSelf(std::ifstream* _file) {
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
	#pragma endregion
#pragma endregion

#pragma region Self-Child Function
	#pragma region Visual
		void ATHElement::Draw() {
			window->setView(*view);

			DrawSelf();

			for (unsigned int i = 0; i < childs.size(); i++) {
				childs[i].Draw();
			}
		}
	#pragma endregion
	#pragma region Childs Vector
		ATHElement* ATHElement::AddElement(ATHElement _element, unsigned int _num) {
			_element.parent = this;
			_element.view = view;
			_element.window = window;

			for (unsigned int i = 0; i < _num; i++)
				childs.push_back(_element);
			return &childs.back();
		}
		ATHElement* ATHElement::AddElement(std::string _name, unsigned int _num) {
			for (unsigned int i = 0; i < _num; i++)
				childs.push_back(ATHElement(*this, _name));
			return childs.size() != 0 ? &childs.back() : NULL;
		}
		ATHElement* ATHElement::AddElement(std::string _name, sf::Vector2f _pos, unsigned int _num) {
			for (unsigned int i = 0; i < _num; i++)
				childs.push_back(ATHElement(*this, _name, _pos));
			return &childs.back();
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

		void ATHElement::Clear() {
			childs.clear();
		}
	#pragma endregion
	#pragma region Files
		void ATHElement::Save(std::string _path, unsigned int _numTab) {
			std::ofstream _file;
			_file.open(_path);
			Save(&_file, _numTab);

			//_file << "childs_size" << '\t' << childs.size() << std::endl;

			//for (unsigned int i = 0; i < childs.size(); i++) {
			//	_file << "----- Element " << i << " : " << childs[i].name << " -----" << std::endl;
			//	childs[i].SaveSelf(&_file);
			//	_file << std::endl << std::endl;
			//}


			_file.close();
		}
		void ATHElement::Save(std::ofstream* _file, unsigned int _numTab) {
			SaveSelf(_file, _numTab);

			*_file << tab(_numTab) << "#pragma region " << name << std::endl;
				for (unsigned int i = 0; i < childs.size(); i++) {
					*_file << tab(_numTab + 1) << name << ".childs[" << i << "]   ";
					childs[i].Save(_file, _numTab + 1);
					*_file << std::endl << std::endl;
				}
			*_file << tab(_numTab) << "#pragma endregion";

		}
		void ATHElement::OldLoad(std::string _path) {
			std::ifstream _file;
			_file.open(_path);

			//Get the number of elements in the file
			const int nbElements = ReadInt(&_file);
			Clear();
			AddElement("", nbElements);
			//childs.resize(nbElements, ATHElement(*this, ""));

			const sf::Vector2f _scales = ReadVector2f(&_file);
			//view.setSize(ReadVector2f(&_file));

			for (unsigned int i = 0; i < childs.size(); i++) {
				childs[i].OldLoadSelf(&_file);
			}

			//view.setSize(_scales);

			_file.close();
		}
		void ATHElement::Load(std::string _path, unsigned int _numTab) {
			std::ifstream _file;
			_file.open(_path);
			Load(&_file, _numTab);
			//Get the number of elements in the file
			//const int nbElements = ReadInt(&_file);
			//Clear();
			//AddElement("", nbElements);
			////childs.resize(nbElements, ATHElement(*this, ""));

			////const sf::Vector2f _scales = ReadVector2f(&_file);
			////view.setSize(ReadVector2f(&_file));

			//for (unsigned int i = 0; i < childs.size(); i++) {
			//	childs[i].LoadSelf(&_file);
			//}

			//view.setSize(_scales);

			_file.close();
		}
		void ATHElement::Load(std::ifstream* _file, unsigned int _numTab) {
			LoadSelf(_file, _numTab);

			ReadSkipTitle(_file);//Skip #pregma region
			for (unsigned int i = 0; i < childs.size(); i++) {
				childs[i].Load(_file, _numTab + 1);
			}
			ReadSkipTitle(_file);//Skip #pregma endregion
		}
	#pragma endregion
	#pragma region Transformable
		void ATHElement::SetPosition(sf::Vector2f _newPos) {
			const sf::Vector2f deltaPos = _newPos - pos;
			Move(deltaPos);
		}
		void ATHElement::SetPosition(float _newPosX, float _newPosY) {
			SetPosition(sf::Vector2f(_newPosX, _newPosY));
		}

		void ATHElement::Move(sf::Vector2f _move) {
			pos += _move;

			for (ATHElement& child : childs) {
				child.Move(_move);
			}
		}
		void ATHElement::Move(float _moveX, float _moveY) {
			Move(sf::Vector2f(_moveX, _moveY));
		}

		void ATHElement::Scale(sf::Vector2f _scale) {
			Scale(_scale.x, _scale.y);
		}
		void ATHElement::Scale(float _scaleX, float _scaleY) {	
			scale.x *= _scaleX;
			scale.y *= _scaleY;

			for (ATHElement& child : childs) {
				child.Scale(_scaleX, _scaleY);
			}
		}
	#pragma endregion
#pragma endregion

#pragma region Global Function
			std::string ATH::ToString(sf::Color _color, char _insertChar) {
				return std::to_string(_color.r) + _insertChar + std::to_string(_color.g) + _insertChar + std::to_string(_color.b) + _insertChar + std::to_string(_color.a);
			}
			std::string ATH::ToString(sf::Vector2f _vector, char _insertChar) {
				return std::to_string(_vector.x) + _insertChar + std::to_string(_vector.y);
			}

			sf::Vector2f ATH::ReadVector2f(std::ifstream* _file) {

				sf::Vector2f _vector2f;
				*_file >> _vector2f.x >> _vector2f.y;
				return _vector2f;
			}
			sf::Color ATH::ReadColor(std::ifstream* _file) {

				int _r, _g, _b, _a;
				*_file >> _r >> _g >> _b >> _a;
				return sf::Color(_r, _g, _b, _a);
			}
			std::string ATH::ReadString(std::ifstream* _file, int _lineSkip, int _charSkip) {

				std::string _string = "";

				for (int i = 0; i < _lineSkip; i++)
					std::getline(*_file, _string);

					_file->ignore(_charSkip);

				std::getline(*_file, _string);

				return _string;
			}
			int ATH::ReadInt(std::ifstream* _file) {

				int _int;
				*_file >> _int;
				return _int;
			}
			void ATH::ReadSkipTitle(std::ifstream* _file) {
				std::string _string;
				*_file >> _string;
				ReadString(_file);
			}
			int ATH::GetKeyPressed() {
				int _keyPressed;

				for (_keyPressed = 0; _keyPressed < sf::Keyboard::Key::KeyCount; _keyPressed++) {
					if (sf::Keyboard::isKeyPressed((sf::Keyboard::Key)_keyPressed)) {
						break;
					}
				}

				return _keyPressed;
			}


			std::string ATH::tab(unsigned int _numTab) {
				std::string _value = "";
				for (unsigned int i = 0; i < _numTab; i++)
					_value += '\t';
				return _value;
			}
#pragma endregion