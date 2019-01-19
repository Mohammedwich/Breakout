// Breakout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>




int main()
{
	sf::RenderWindow mainWindow(sf::VideoMode(600, 900), "Breakout", sf::Style::Default);
	sf::VertexArray ballDeflecter;

	while (mainWindow.isOpen())
	{
		sf::Event event;

		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mainWindow.close();
			}


		}

	}

	mainWindow.clear(sf::Color::Black);
	//mainWindow.draw(ballDeflector);
	mainWindow.display();


	return 0;
}

