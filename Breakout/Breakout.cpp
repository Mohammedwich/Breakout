// Breakout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Brick.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

using std::cin;
using std::cout;
using std::endl;


int main()
{
	sf::ContextSettings context;
	context.antialiasingLevel = 8;

	sf::RenderWindow mainWindow(sf::VideoMode(550, 600), "Breakout", sf::Style::Default, context);

	sf::Texture galaxyTexture;
	if (!galaxyTexture.loadFromFile("Galaxy.jpg"))
	{
		cout << "Failed to load for galaxyTexture" << endl;
	}

	sf::Sprite background;
	background.setTexture(galaxyTexture);
	background.setScale(2, 2);

	// Game's borders
	sf::RectangleShape borderUp;
	borderUp.setSize(sf::Vector2f(550, 1) );
	borderUp.setPosition(sf::Vector2f(0, 0) );
	borderUp.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderUpBound = borderUp.getGlobalBounds();

	sf::RectangleShape borderDown;
	borderDown.setSize(sf::Vector2f(550, 1));
	borderDown.setPosition(sf::Vector2f(0, 600));
	borderDown.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderDownBound = borderDown.getGlobalBounds();

	sf::RectangleShape borderLeft;
	borderLeft.setSize(sf::Vector2f(1, 600));
	borderLeft.setPosition(sf::Vector2f(0, 0));
	borderLeft.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderLeftBound = borderLeft.getGlobalBounds();

	sf::RectangleShape borderRight;
	borderRight.setSize(sf::Vector2f(1, 600));
	borderRight.setPosition(sf::Vector2f(550, 0));
	borderRight.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderRightBound = borderRight.getGlobalBounds();

	// Making the pad
	sf::ConvexShape ballDeflector;
	ballDeflector.setPointCount(6);
	ballDeflector.setPoint(0, sf::Vector2f(225.f, 590.f));
	ballDeflector.setPoint(1, sf::Vector2f(237.f, 582.f));
	ballDeflector.setPoint(2, sf::Vector2f(253.f, 577.f));
	ballDeflector.setPoint(3, sf::Vector2f(295.f, 577.f));
	ballDeflector.setPoint(4, sf::Vector2f(313.f, 582.f));
	ballDeflector.setPoint(5, sf::Vector2f(325.f, 590.f));

	ballDeflector.setFillColor(sf::Color::Yellow);
	ballDeflector.setTexture(&galaxyTexture);
	ballDeflector.setTextureRect(sf::IntRect(145, 180, 250, 200));

	// The bricks and their positions on the window
	std::vector<Brick> brickVector(50);

	for (int row = 0; row < 5; ++row)
	{
		for (int column = 0; column < 10; ++column)
		{
			brickVector[(row * 10) + column].setPosition(sf::Vector2f( (1+ column*55.f), (1 + row*19.f) ));
		}
	}



	while (mainWindow.isOpen())
	{
		sf::Event event;

		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mainWindow.close();
			}

			//Move the ballDeflector
			if (event.type == sf::Event::KeyPressed)
			{
				if ( (event.key.code == sf::Keyboard::Left) && (borderLeftBound.intersects(ballDeflector.getGlobalBounds()) == false))
				{
					ballDeflector.move(-5, 0);
				}

				if ((event.key.code == sf::Keyboard::Right) && (borderRightBound.intersects(ballDeflector.getGlobalBounds()) == false))
				{
					ballDeflector.move(5, 0);
				}
			}


		}

		mainWindow.clear(sf::Color::Black);

		for (Brick & theBrick : brickVector)
		{
			if (theBrick.isBroken() == false)
			{
				mainWindow.draw(theBrick);
			}
			else
			{
				theBrick.drawItsPowerUp(mainWindow);
			}
		}

		mainWindow.draw(ballDeflector);
		mainWindow.display();
	}

	
	return 0;
}

