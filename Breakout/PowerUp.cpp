#include "stdafx.h"
#include "PowerUp.h"


PowerUp::PowerUp()
{
	sf::RectangleShape::setSize(sf::Vector2f(25.f, 9.f));
	itsPower = 0;
}


PowerUp::~PowerUp()
{
}

/*
sf::Vector2f PowerUp::getPosition()
{
	return itsPosition;
}
*/

/*
void PowerUp::setPosition(sf::Vector2f thePosition)
{
	itsPosition = thePosition;
}
*/

void PowerUp::fallDown()
{
	sf::RectangleShape::move(0, 4);
}

void PowerUp::draw(sf::RenderWindow & theWindow)
{
	theWindow.draw(*this);
	fallDown();
}

void PowerUp::setPower(int theInt)
{
	itsPower = theInt;
}
