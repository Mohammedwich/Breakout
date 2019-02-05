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


void PowerUp::fallDown(double fallFactor)
{
	sf::RectangleShape::move(0, (1.5 * fallFactor) );
}

void PowerUp::draw(sf::RenderWindow & theWindow, double fallFactor)
{
	theWindow.draw(*this);
	fallDown(fallFactor);
}

void PowerUp::setPower(int theInt)
{
	itsPower = theInt;
}
