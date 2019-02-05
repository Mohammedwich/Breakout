#include "stdafx.h"
#include "Ball.h"


Ball::Ball() : edgePoints(8, sf::Vector2f(0.f, 0.f))
{
	std::uniform_int_distribution<int> startingAngleDist(45, 135);	//Launch angle from deflector
	
	itsSpeed = 2.6f;
	stuck = true;
	int randomAngle = startingAngleDist(ballRanDev);
	itsAngle = randomAngle * (2 * std::_Pi / 360);

	sf::CircleShape::setRadius(10);
	sf::CircleShape::setPointCount(7);
	sf::CircleShape::setFillColor(sf::Color(201, 255, 10));
	sf::CircleShape::setOrigin(getRadius(), getRadius());

	
}


Ball::~Ball()
{
}

bool Ball::isStuck()
{
	return stuck;
}

void Ball::stick()
{
	stuck = true;
}

void Ball::unStick()
{
	stuck = false;
}

float Ball::getSpeed()
{
	return itsSpeed;
}

void Ball::setSpeed(float multiple)
{
	itsSpeed *= multiple;
}

long double Ball::getAngle()
{
	return itsAngle;
}

void Ball::setAngle(long double direction)
{
	itsAngle = direction;
}

std::vector<sf::Vector2f> & Ball::getEdgePoints()
{
	// [0] is the left point. This moves clockwise
	edgePoints[0] = sf::Vector2f((sf::CircleShape::getPosition().x - sf::CircleShape::getRadius()), (sf::CircleShape::getPosition().y));

	edgePoints[1] = sf::Vector2f( (sf::CircleShape::getPosition().x - (sf::CircleShape::getRadius() * cos(std::_Pi / 4)) ),
								(sf::CircleShape::getPosition().y - (sf::CircleShape::getRadius() * sin(std::_Pi / 4)) ));

	edgePoints[2] = sf::Vector2f((sf::CircleShape::getPosition().x), (sf::CircleShape::getPosition().y - sf::CircleShape::getRadius()));

	edgePoints[3] = sf::Vector2f((sf::CircleShape::getPosition().x + (sf::CircleShape::getRadius() * cos(std::_Pi / 4))),
								(sf::CircleShape::getPosition().y - (sf::CircleShape::getRadius() * sin(std::_Pi / 4))));

	edgePoints[4] = sf::Vector2f((sf::CircleShape::getPosition().x + sf::CircleShape::getRadius()), (sf::CircleShape::getPosition().y));

	edgePoints[5] = sf::Vector2f((sf::CircleShape::getPosition().x + (sf::CircleShape::getRadius() * cos(std::_Pi / 4))),
								(sf::CircleShape::getPosition().y + (sf::CircleShape::getRadius() * sin(std::_Pi / 4))));

	edgePoints[6] = sf::Vector2f((sf::CircleShape::getPosition().x), (sf::CircleShape::getPosition().y + sf::CircleShape::getRadius()));

	edgePoints[7] = sf::Vector2f((sf::CircleShape::getPosition().x - (sf::CircleShape::getRadius() * cos(std::_Pi / 4))),
								(sf::CircleShape::getPosition().y + (sf::CircleShape::getRadius() * sin(std::_Pi / 4))));

	return edgePoints;
}

bool Ball::isDead()
{
	if (dead == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Ball::kill()
{
	dead = true;
}
