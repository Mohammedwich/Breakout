#include "stdafx.h"
#include "Brick.h"



Brick::Brick()
{
	sf::RectangleShape::setSize(sf::Vector2f(53.f, 18.f));
	sf::RectangleShape::setFillColor(sf::Color::Blue);

	std::uniform_int_distribution<int> chanceDist(0, 9);
	std::uniform_int_distribution<int> powerDist(1, 3);

	int willItPower = chanceDist(ranDev);
	//30% chance a brick will have a powerUp
	if (willItPower >= 0 && willItPower <= 2)
	{
		hasPowerUp = true;

		//Randomly pick which power the brick has
		itsPower = powerDist(ranDev);

		sf::RectangleShape::setFillColor(sf::Color::Red);
	}


	//Setting up the powerUp within
	itsPowerUp.setPosition(getPosition());
	itsPowerUp.move(4, 0);
	itsPowerUp.setPower(itsPower);

	switch (itsPower)
	{
	case MAGNETIC:
		itsPowerUp.setFillColor(sf::Color::Magenta);
		break;

	case BOMB:
		itsPowerUp.setFillColor(sf::Color::Black);
		break;

	case EXTRABALL:
		itsPowerUp.setFillColor(sf::Color::White);
		break;

	default:
		break;
	}

}


Brick::~Brick()
{
}

bool Brick::isBroken()
{
	if (broken == false)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Brick::crush()
{
	broken = true;
}

int Brick::whichPower()
{
	return itsPower;
}

void Brick::drawItsPowerUp(sf::RenderWindow & theWindow)
{
	itsPowerUp.draw(theWindow);
}
