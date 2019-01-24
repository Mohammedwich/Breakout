#pragma once
#include <SFML\Graphics.hpp>
#include <random>

static std::random_device ballRanDev;

class Ball : public sf::CircleShape
{
public:
	Ball();
	~Ball();

	bool isStuck();
	void stick();
	void unStick();
	float getSpeed();
	void setSpeed(float);
	long double getAngle();
	void setAngle(long double);

	static int numberOfBalls;
	

private:
	float itsSpeed;
	bool stuck;
	long double itsAngle;
};

