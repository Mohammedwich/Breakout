#include "stdafx.h"
#include "Brick.h"



Brick::Brick()
{
	sf::RectangleShape::setSize(sf::Vector2f(53.f, 18.f));
	sf::RectangleShape::setFillColor(sf::Color::Blue);

	std::uniform_int_distribution<int> chanceDist(0, 9);
	std::uniform_int_distribution<int> powerDist(1, 3);

	int willItPower = chanceDist(brickRanDev);
	//30% chance a brick will have a powerUp
	if (willItPower >= 0 && willItPower <= 2)
	{
		hasPowerUp = true;

		//Randomly pick which power the brick has
		itsPower = powerDist(brickRanDev);

		sf::RectangleShape::setFillColor(sf::Color::Red);
	}


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


	if (!breakBuffer.loadFromFile("Brick break.wav"))
	{
		std::cout << "Failed to load for detonationBuffer" << std::endl;
	}

	breakSound.setBuffer(breakBuffer);
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
	breakSound.play();
}

int Brick::whichPower()
{
	return itsPower;
}

void Brick::drawItsPowerUp(sf::RenderWindow & theWindow, double fallFactor)
{
	itsPowerUp.draw(theWindow, fallFactor);
}

sf::Vector2f Brick::getPowerUpPosition()
{
	return itsPowerUp.getPosition();
}

sf::FloatRect Brick::getPowerUpBounds()
{
	return itsPowerUp.getGlobalBounds();
}

void Brick::setPowerUpPosition(sf::Vector2f position)
{
	itsPowerUp.setPosition(position);
}

void Brick::setPowerUpPosition(sf::Vector2f position, float xMove, float yMove)
{
	itsPowerUp.setPosition(position);
	itsPowerUp.move(xMove, yMove);
}

void Brick::syncPowerUpPosition()
{
	itsPowerUp.setPosition((*this).getPosition());
	itsPowerUp.move(15, 0);
}