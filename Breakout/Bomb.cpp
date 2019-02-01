#include "stdafx.h"
#include "Bomb.h"


Bomb::Bomb()
{
	setFillColor(sf::Color(139, 139, 147));
	setRadius(7);
	setOrigin(getRadius(), getRadius() );
}


Bomb::~Bomb()
{
}

bool Bomb::isFlying()
{
	if (inFlight == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Bomb::stopFlight()
{
	inFlight = false;
}

void Bomb::shoot()
{
	inFlight = true;
}

void Bomb::detonate()
{
	// All this stuff is to correct positioning when radius is changed and moved relative to origin
	sf::Vector2f currentPosition = getPosition();
	setOrigin(0, 0);
	setPosition(0, 0);
	setRadius(30);
	setOrigin(getRadius(), getRadius());
	move(currentPosition);

	setFillColor(sf::Color(0, 0, 0));
	detonated = true;
	//inFlight = false;
}

bool Bomb::isDetonated()
{
	if (detonated == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Bomb::draw(sf::RenderWindow & theWindow, double speed)
{
	theWindow.draw(*this);

	if (detonated == false)
	{
		sf::CircleShape::move(0, -speed);
	}
	rotate(0.5);
}

void Bomb::revertDetonation()
{
	// All this stuff is to correct positioning when radius is changed and moved relative to origin
	sf::Vector2f currentPosition = getPosition();
	setOrigin(0,0);
	setPosition(0,0);
	setRadius(7);
	setOrigin(getRadius(), getRadius());
	move(currentPosition);

	setFillColor(sf::Color(139, 139, 147));
	detonated = false;
}