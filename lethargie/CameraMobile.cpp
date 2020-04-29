#include "CameraMobile.h"

void CameraMobile::move(Float2 objectif, float constTemps)
{
	float temps = horloge.restart().asSeconds()/constTemps;
	if (temps > 1)
	{
		setCenter(objectif);
	}
	else
	{
		Float2 delta = objectif - getCenter();
		const float distance = delta.norm();
		const float facteur = (-100 / (distance + 100)) + 1;
		setCenter(getCenter() + delta * facteur);
	}
}