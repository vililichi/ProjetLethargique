#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FichierIO.h"

class ATHElement {
public:
	////Var

	sf::Vector2f pos;

	//Render
	sf::RenderWindow* window;
	sf::View* view; //position and scale of the entire ATH

	//Children-Parent Relation
	std::vector<ATHElement> childs;
	ATHElement* parent;

	//Visual
	sf::RectangleShape background;
	sf::Text contentText, defaultText;

	//Identification
	std::string name;

	int prevKey;

	bool toggle, isChangingTextByUser;

	///Constructor
	//Empty - Cant be draw cause no windows and view
	ATHElement(std::string _name = "", sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _size = sf::Vector2f(0.0f, 0.0f));

	//child - Cant work without a parent who have a window and a view 
	ATHElement(ATHElement& _parent, std::string _name, sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _size = sf::Vector2f(0.0f, 0.0f));

	//parent - Work alone
	ATHElement(sf::RenderWindow& _window, sf::View& _view, std::string _name = "", sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _size = sf::Vector2f(0.0f, 0.0f));
	
	//Self Function
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

	void DrawSelf();

	void SaveSelf(std::string _path);
	void SaveSelf(std::ofstream *_file);
	void LoadSelf(std::string _path);
	void LoadSelf(std::ifstream *_file);

	//Self-Child Function
	void Clear();
	//ATHElement* AddElement();
	ATHElement* AddElement(ATHElement _element);
	ATHElement* AddElement(std::string _name = "", sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _size = sf::Vector2f(0.0f, 0.0f));

	void Save(std::string _path);
	void Load(std::string _path);

	ATHElement* FindElement(std::string _name);

	void SetPosition(sf::Vector2f _newPos);
	void SetPosition(float _newPosX, float _newPosY);

	void Move(sf::Vector2f _move);
	void Move(float _moveX, float _moveY);

	//Deprecated use Scale instead
	//void Resize(sf::Vector2f _newSize);
	//void Resize(unsigned int _sizeX, unsigned int _sizeY);

	void Scale(sf::Vector2f _scale);
	void Scale(float _scaleX, float _scaleY);

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