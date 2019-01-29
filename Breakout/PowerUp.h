#pragma once
#include <SFML/Graphics.hpp>

class PowerUp : public sf::RectangleShape
{
public:
	PowerUp();
	~PowerUp();

	//sf::Vector2f getPosition();
	//void setPosition(sf::Vector2f);
	void fallDown(double);
	void draw(sf::RenderWindow & theWindow, double);
	void setPower(int);

private:
	int itsPower;
	//sf::Vector2f itsPosition = sf::Vector2f(0.f, 0.f);
};

