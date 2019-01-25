#include "stdafx.h"
#include "Ball.h"

int Ball::numberOfBalls = 0;

Ball::Ball()
{
	std::uniform_int_distribution<int> startingAngleDist(45, 135);	//Launch angle from deflector
	
	itsSpeed = 0.35f;
	stuck = true;
	int randomAngle = startingAngleDist(ballRanDev);
	itsAngle = randomAngle * (2 * std::_Pi / 360);

	sf::CircleShape::setRadius(10);
	sf::CircleShape::setPointCount(7);
	sf::CircleShape::setFillColor(sf::Color(201, 255, 10));
	sf::CircleShape::setOrigin(getRadius(), getRadius());

	++numberOfBalls;
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
