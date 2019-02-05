#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <random>
#include <vector>
#include <cmath>

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
	std::vector<sf::Vector2f> & getEdgePoints();
	bool isDead();
	void kill();
	

private:
	float itsSpeed;
	bool stuck;
	long double itsAngle;
	std::vector<sf::Vector2f> edgePoints;
	bool dead = false;
};

