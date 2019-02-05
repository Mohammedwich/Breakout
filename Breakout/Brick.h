#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <random>
#include "PowerUp.h"

static std::random_device brickRanDev;
enum Power { NONE = 0, MAGNETIC = 1, BOMB = 2, EXTRABALL = 3};


class Brick : public sf::RectangleShape
{
public:
	Brick();
	~Brick();

	bool isBroken();
	void crush();
	int whichPower();
	void drawItsPowerUp(sf::RenderWindow & theWindow, double);

	sf::Vector2f getPowerUpPosition();
	sf::FloatRect getPowerUpBounds();
	void setPowerUpPosition(sf::Vector2f);
	void setPowerUpPosition(sf::Vector2f, float xMove, float yMove);
	void syncPowerUpPosition();

private:
	bool hasPowerUp = false;
	int itsPower = 0;	
	bool broken = false;
	PowerUp itsPowerUp;

	sf::SoundBuffer breakBuffer;
	sf::Sound breakSound;
};

