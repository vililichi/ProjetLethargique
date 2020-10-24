#pragma once
#include"Math_addOn.h"
#include<SFML/System/Clock.hpp>
#include<SFML/Graphics.hpp>

class CameraMobile : public sf::View
{
private:
	sf::Clock horloge;
public:
	void move(Float2 objectif, float constTemps);
};
