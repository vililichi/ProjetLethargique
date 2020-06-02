#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FichierIO.h"

class ATHElement {
public:
	sf::RenderWindow* window;
	sf::RectangleShape background;
	sf::Text contentText, defaultText;

	std::string name;
	//std::vector<ATHElement> childs; 
	//*ATHElement parent;

	int prevKey;

	bool toggle, isChangingTextByUser;
	ATHElement(std::string _name = "");
	ATHElement(sf::RenderWindow& _window, std::string _name = "");
	
	void SetDefaultText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font &_font);
	void LoadDefaultText(std::ifstream* _file);

	void SetContentText(sf::Vector2f _pos, float _size, sf::Color _color, std::string _text, sf::Font &_font);
	void LoadContentText(std::ifstream* _file);

	void SetBackground(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _color, float _outlineSize, sf::Color _outlineColor);
	void LoadBackground(std::ifstream* _file);

	bool isClicked(int _button, bool _toggle);
	bool isClicked(sf::Vector2i _mousePos, int _button = 0, bool _toggle = false);
	bool isHovered();
	bool isHovered(sf::Vector2i _mousePos);

	void ChangeTextByUser(int _mouseButton = 0, bool _toggle = false, int _sendKey = sf::Keyboard::Enter);
	void ChangeTextByUser(sf::Vector2i _mousePos, int _mouseButton = 0, bool _toggle = false, int _sendKey = sf::Keyboard::Enter);
	std::string WriteText();

	void Draw();

	void Save(std::string _path);
	void Save(std::ofstream *_file);
	void Load(std::string _path);
	void Load(std::ifstream *_file);
};

class ATHManager {
public:
	sf::RenderWindow* window;
	sf::View view; //position and scale of the entire ATH

	std::vector<ATHElement> elements;

	ATHManager();
	ATHManager(sf::RenderWindow& _window, sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _size = sf::Vector2f(0.0f, 0.0f));

	void Clear();
	void AddElement();
	void AddElement(ATHElement _element);

	void Save(std::string _path);
	void Load(std::string _path);

	ATHElement *FindElement(std::string _name);

	void SetPosition(sf::Vector2f _pos);
	void SetPosition(int _posX, int _posY);

	void Move(sf::Vector2f _move);
	void Move(int _moveX, int _moveY);

	void Resize(sf::Vector2f _newSize);
	void Resize(unsigned int _sizeX, unsigned int _sizeY);

	void Scale(float _scale);

	void Draw();
};


static std::string ToString(sf::Color _color, char _insertChar = '\t');
static std::string ToString(sf::Vector2f _vector, char _insertChar = '\t');

sf::Vector2f ReadVector2f(std::ifstream *_file);
sf::Color ReadColor(std::ifstream *_file);
int ReadInt(std::ifstream *_file);
int GetKeyPressed();
std::string ReadString(std::ifstream *_file, int _lineSkip = 0);
void ReadSkipTitle(std::ifstream* _file);