// Breakout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Brick.h"
#include "Ball.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>

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

	// Making background
	sf::Sprite background;
	background.setTexture(galaxyTexture);
	background.setScale(2, 2);
	background.setColor(sf::Color(255, 255, 255, 220));

	// Setting game's borders
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

	// Making the pad(Deflector)
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
	sf::Vector2f deflectorPosition(275.f, 577.f);	//Made to use with ball positioning
	float deflectorWidth = 100;


	// The bricks and their positions on the window
	std::vector<Brick> brickVector(50);

	for (int row = 0; row < 5; ++row)
	{
		for (int column = 0; column < 10; ++column)
		{
			brickVector[(row * 10) + column].setPosition(sf::Vector2f( (1+ column*55.f), (1 + row*19.f) ));
		}
	}

	//The ball
	Ball meteor;
	meteor.move(deflectorPosition.x, deflectorPosition.y - meteor.getRadius() - 1);//set initial position on top of deflector
	float speed = meteor.getSpeed();

	//Texts
	sf::Font sansation;
	if (!sansation.loadFromFile("sansation.ttf") )
	{
		cout << "Error: sansation.ttf failed to load" << endl;
	}

	sf::Text startText;
	startText.setFont(sansation);
	startText.setCharacterSize(20);
	startText.setFillColor(sf::Color(193, 129, 112));
	startText.setOutlineThickness(5);
	startText.setOutlineColor(sf::Color::Black);
	startText.setPosition(75, 250);
	startText.setString("Use the left and right keys to move the deflector.\n"
						"Use the up key to release the ball.\n"
						"To pause and un-pause click the space bar.\n\n"
						"To start the game, hit Enter.\n");

	sf::Text pauseText;
	pauseText.setFont(sansation);
	pauseText.setCharacterSize(30);
	pauseText.setFillColor(sf::Color(193, 129, 112));
	pauseText.setOutlineThickness(5);
	pauseText.setOutlineColor(sf::Color::Black);
	pauseText.setPosition(225, 250);
	pauseText.setString("Paused");

	sf::Text loseText;
	loseText.setFont(sansation);
	loseText.setCharacterSize(40);
	loseText.setFillColor(sf::Color(193, 129, 112));
	loseText.setOutlineThickness(5);
	loseText.setOutlineColor(sf::Color::Black);
	loseText.setPosition(210, 250);
	loseText.setString("You Lose");

	sf::Text playAgain;
	playAgain.setFont(sansation);
	playAgain.setCharacterSize(20);
	playAgain.setFillColor(sf::Color(175, 149, 128));
	playAgain.setOutlineThickness(5);
	playAgain.setOutlineColor(sf::Color::Black);
	playAgain.setPosition(200, 320);
	playAgain.setString("Press 'P' to play again.");

	//Time 
	sf::Clock theClock;
	sf::Time timePassed = theClock.getElapsedTime();

	//Game conditions
	bool startScreen = true;	//Used to show instructions before starting game
	bool paused = false;	
	bool pause_unpause_inhibitor = false;	//Used to prevent instant unpause after a pause in the Space event pause condition below.
	bool gameLost = false;


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
				//Until the start screen is bypassed with enter, nothing will be controlled
				if (event.key.code == sf::Keyboard::Enter)
				{
					startScreen = false;
				}

				//While paused all objects stop updating until unpaused. They resume from their pre-paused state
				if (startScreen == false)
				{
					if (event.key.code == sf::Keyboard::Space)
					{
						if (gameLost == false && paused == false)
						{
							paused = true;
							pause_unpause_inhibitor = true;
						}
						if (gameLost == false && paused == true && pause_unpause_inhibitor == false)
						{
							paused = false;
						}
						pause_unpause_inhibitor = false;
					}
				}

				// Controls
				if (paused == false && startScreen == false)
				{
					if ((event.key.code == sf::Keyboard::Left) && (borderLeftBound.intersects(ballDeflector.getGlobalBounds()) == false))
					{
						ballDeflector.move(-10, 0);
						deflectorPosition.x -= 10;
						if (meteor.isStuck())
						{
							meteor.move(-10, 0);
						}
					}

					if ((event.key.code == sf::Keyboard::Right) && (borderRightBound.intersects(ballDeflector.getGlobalBounds()) == false))
					{
						ballDeflector.move(10, 0);
						deflectorPosition.x += 10;
						if (meteor.isStuck())
						{
							meteor.move(10, 0);
						}
					}

					if ((event.key.code == sf::Keyboard::Up) && (meteor.isStuck()))
					{
						meteor.unStick();
					}

					if ((event.key.code == sf::Keyboard::P) && gameLost == true)
					{
						deflectorPosition = sf::Vector2f(275.f, 577.f);
						ballDeflector.setPosition(0, 0);

						meteor.setPosition(0, 0);
						meteor.move(deflectorPosition.x, deflectorPosition.y - meteor.getRadius() - 1);
						std::uniform_int_distribution<int> resetAngleDist(45, 135);
						int randomAngle = resetAngleDist(ballRanDev);
						meteor.setAngle(randomAngle * (2 * std::_Pi / 360));

						gameLost = false;
						meteor.stick();
					}
				}
			}

		}

		
		//Moving the ball
		if (!meteor.isStuck() && gameLost == false && paused == false && startScreen == false)
		{
			meteor.move(speed*(cos(meteor.getAngle())), -speed*( sin(meteor.getAngle())));

			if (borderLeftBound.contains((meteor.getPosition().x - meteor.getRadius()), meteor.getPosition().y))
			{
				meteor.setAngle(std::_Pi - meteor.getAngle());
			}

			if (borderRightBound.contains((meteor.getPosition().x + meteor.getRadius()), meteor.getPosition().y))
			{
				meteor.setAngle(std::_Pi - meteor.getAngle());
			}

			if (borderUpBound.contains((meteor.getPosition().x), meteor.getPosition().y - meteor.getRadius()))
			{
				meteor.setAngle(0 - meteor.getAngle());
			}

			if (borderDownBound.contains((meteor.getPosition().x), meteor.getPosition().y + meteor.getRadius()))
			{
				gameLost = true;
			}

			//Ball bouncing from deflector
			if ((meteor.getPosition().x >= (deflectorPosition.x - deflectorWidth / 2)) &&
				(meteor.getPosition().x <= (deflectorPosition.x + deflectorWidth / 2)) &&
				((meteor.getPosition().y + meteor.getRadius()) >= deflectorPosition.y) &&
				(gameLost == false )													)
			{
				//play sound here
				meteor.setAngle(0 - meteor.getAngle());
			}

		}
		
		

		// Draw Start screen
		if (gameLost == false && paused == false && startScreen == true)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);
			mainWindow.draw(startText);
			mainWindow.display();
		}

		// Draw gameplay
		if (gameLost == false && paused == false && startScreen == false)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);

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
			mainWindow.draw(meteor);
			mainWindow.display();
		}

		// Draw lose screen
		if (gameLost == true && paused == false && startScreen == false)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);
			mainWindow.draw(loseText);
			mainWindow.draw(playAgain);
			mainWindow.display();
		}

		// Draw pause screen
		if (gameLost == false && paused == true && startScreen == false)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);

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
			mainWindow.draw(meteor);
			mainWindow.draw(pauseText);
			mainWindow.display();
		}
	}

	
	return 0;
}

