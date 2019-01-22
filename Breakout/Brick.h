#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "PowerUp.h"

static std::random_device ranDev;
enum Power { NONE = 0, MAGNETIC = 1, BOMB = 2, EXTRABALL = 3};


class Brick : public sf::RectangleShape
{
public:
	Brick();
	~Brick();

	bool isBroken();
	void crush();
	int whichPower();
	void drawItsPowerUp(sf::RenderWindow & theWindow);

private:
	bool hasPowerUp = false;
	int itsPower = 0;	
	bool broken = false;
	PowerUp itsPowerUp;
};

