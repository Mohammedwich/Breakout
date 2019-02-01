#pragma once
#include <sfml/Graphics.hpp>

class Bomb : public sf::CircleShape
{
public:
	Bomb();
	~Bomb();

	bool isFlying();
	void stopFlight();
	void shoot();
	void detonate();
	bool isDetonated();
	void draw(sf::RenderWindow &, double);
	void revertDetonation();
	

private:
	bool detonated = false;
	bool inFlight = false;
};

