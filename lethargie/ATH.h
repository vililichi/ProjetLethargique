#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FichierIO.h"

class ATHElement {
public:
	#pragma region Variables
		#pragma region Transformable
			sf::Vector2f pos, scale;
		#pragma endregion
		#pragma region Visual
				sf::RenderWindow* window;
				sf::View* view;

				sf::RectangleShape background;
				sf::Text contentText, defaultText;
		#pragma endregion
		#pragma region Hierarchy
				std::vector<ATHElement> childs;
				ATHElement* parent;

				std::string name;
		#pragma endregion
		#pragma region Elements Actions
				int prevKey;
				bool toggle, isChangingTextByUser;
		#pragma endregion
	#pragma endregion

	#pragma region Constructor
		#pragma region Empty - Cant be draw cause no windows and view
			ATHElement(std::string _name = "", sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _scale = sf::Vector2f(1, 1));
		#pragma endregion

		#pragma region Child - Cant work without a parent who have a window and a view 
			ATHElement(ATHElement& _parent, std::string _name, sf::Vector2f _pos);
			ATHElement(ATHElement& _parent, std::string _name);
		#pragma endregion

		#pragma region Parent - Work alone
			ATHElement(sf::RenderWindow& _window, sf::View& _view, std::string _name = "", sf::Vector2f _pos = sf::Vector2f(0.0f, 0.0f), sf::Vector2f _scale = sf::Vector2f(1, 1));
		#pragma endregion
	#pragma endregion

	#pragma region Self function
		#pragma region Visual
			void SetDefaultText(sf::Vector2f _pos, unsigned int _size, sf::Color _color, std::string _text, sf::Font& _font);
			void SetContentText(sf::Vector2f _pos, unsigned int _size, sf::Color _color, std::string _text, sf::Font& _font);
			void SetBackground(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _color, float _outlineSize, sf::Color _outlineColor);
			
			void DrawSelf();
		#pragma endregion
		#pragma region Elements Actions
			bool isClicked(int _button, bool _toggle);
			bool isClicked(sf::Vector2i _mousePos, int _button = 0, bool _toggle = false);

			bool isHovered();
			bool isHovered(sf::Vector2i _mousePos);

			void ChangeTextByUser(int _mouseButton = 0, bool _toggle = false, int _sendKey = sf::Keyboard::Enter);
			void ChangeTextByUser(sf::Vector2i _mousePos, int _mouseButton = 0, bool _toggle = false, int _sendKey = sf::Keyboard::Enter);

			std::string WriteText();
		#pragma endregion
		#pragma region Files
			void LoadDefaultText(std::ifstream* _file, unsigned int _numTab = 0);
			void LoadContentText(std::ifstream* _file, unsigned int _numTab = 0);
			void LoadBackground(std::ifstream* _file);

			//void SaveSelf(std::string _path);
			void SaveSelf(std::ofstream* _file, unsigned int _numTab = 0);

			//void LoadSelf(std::string _path);
			void LoadSelf(std::ifstream* _file, unsigned int _numTab = 0);
			void OldLoadSelf(std::ifstream* _file);
		#pragma endregion
	#pragma endregion
	#pragma region Self-Child function
		#pragma region Visual
			void Draw();
		#pragma endregion
		#pragma region Childs Vector
			ATHElement* AddElement(ATHElement _element, unsigned int _num = 1);
			ATHElement* AddElement(std::string _name = "", unsigned int _num = 1);
			ATHElement* AddElement(std::string _name, sf::Vector2f _pos, unsigned int _num = 1);

			ATHElement* FindElement(std::string _name);

			void Clear();
		#pragma endregion
		#pragma region Files
			void Save(std::string _path, unsigned int _numTab = 0);
			void Save(std::ofstream* _file, unsigned int _numTab = 0);
			
			void OldLoad(std::string _path);
			void Load(std::string _path, unsigned int _numTab = 0);
			void Load(std::ifstream* _file, unsigned int _numTab = 0);
		#pragma endregion
		#pragma region Transformable
			void SetPosition(sf::Vector2f _newPos);
			void SetPosition(float _newPosX, float _newPosY);

			void Move(sf::Vector2f _move);
			void Move(float _moveX, float _moveY);

			void Scale(sf::Vector2f _scale);
			void Scale(float _scaleX, float _scaleY);
		#pragma endregion
	#pragma endregion
};
namespace ATH {
	#pragma region Global Function
		static std::string ToString(sf::Color _color, char _insertChar = '\t');
		static std::string ToString(sf::Vector2f _vector, char _insertChar = '\t');

		sf::Vector2f ReadVector2f(std::ifstream* _file);
		sf::Color ReadColor(std::ifstream* _file);
		int ReadInt(std::ifstream* _file);
		int GetKeyPressed();
		std::string ReadString(std::ifstream* _file, int _lineSkip = 0, int _charSkip = 0);
		void ReadSkipTitle(std::ifstream* _file);

		std::string tab(unsigned int _numTab);
		std::string tab(unsigned int _numTab);
	#pragma endregion
}