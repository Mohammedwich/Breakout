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

	std::uniform_int_distribution<int> resetAngleDist(45, 135);	// Used for setting ball starter angles

	sf::ContextSettings context;
	context.antialiasingLevel = 8;

	sf::RenderWindow mainWindow(sf::VideoMode(550, 600), "Breakout", sf::Style::Close | sf::Style::Titlebar, context);

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
	float deflectorHight = 13;
	float deflectorSpeed = 0.5;
	
	bool deflectorMagnetized = false;
	int bombAmmo = 0;


	// The bricks and their positions on the window
	std::vector<Brick> brickVector(50);

	for (int row = 0; row < 5; ++row)
	{
		for (int column = 0; column < 10; ++column)
		{
			brickVector[(row * 10) + column].setPosition(sf::Vector2f( (1+ column*55.f), (1 + row*19.f) ));
			brickVector[(row * 10) + column].syncPowerUpPosition();
		}
	}

	//The ball
	std::vector<Ball> ballVector(1);
	ballVector[0].move(deflectorPosition.x, deflectorPosition.y - ballVector[0].getRadius() - 1);//set initial position on top of deflector
	float speed = ballVector[0].getSpeed();

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
	loseText.setString("You Lost");
	
	sf::Text winText;
	winText.setFont(sansation);
	winText.setCharacterSize(40);
	winText.setFillColor(sf::Color(193, 129, 112));
	winText.setOutlineThickness(5);
	winText.setOutlineColor(sf::Color::Black);
	winText.setPosition(170, 250);
	winText.setString("You Won (/^_^)/");
	
	sf::Text playAgain;
	playAgain.setFont(sansation);
	playAgain.setCharacterSize(20);
	playAgain.setFillColor(sf::Color(175, 149, 128));
	playAgain.setOutlineThickness(5);
	playAgain.setOutlineColor(sf::Color::Black);
	playAgain.setPosition(200, 320);
	playAgain.setString("Press 'P' to play again.");


	//Game Variables
	bool startScreen = true;	//Used to show instructions before starting game
	bool paused = false;	
	bool pause_unpause_inhibitor = false;	//Used to prevent instant unpause after a pause in the Space event pause condition below.
	bool gameLost = false;
	bool gameWon = false;
	int brokenBricks = 0;
	float initialBallSpeed = ballVector[0].getSpeed();
	int numberOfBalls = 1;

	//Time 
	sf::Clock theClock;
	


	while (mainWindow.isOpen())
	{
		sf::Event event;
		for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
		{
			if ((*ballIter).isDead() == false)
			{
				if ((*ballIter).isStuck())
				{
					deflectorSpeed = 0.5;
				}
				else
				{
					deflectorSpeed = 0.8;
				}
			}
		}

		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mainWindow.close();
			}

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
						if (gameLost == false && gameWon == false && paused == false)
						{
							paused = true;
							pause_unpause_inhibitor = true;
						}
						if (gameLost == false && gameWon == false && paused == true && pause_unpause_inhibitor == false)
						{
							paused = false;
						}

						pause_unpause_inhibitor = false;
					}
				}

				// Some Controls
				if (paused == false && startScreen == false)
				{
					if ((event.key.code == sf::Keyboard::Up) )
					{
						for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
						{
							if ((*ballIter).isDead() == false)
							{
								if ((*ballIter).isStuck())
								{
									(*ballIter).unStick();
								}
							}
						}
					}

					if (event.key.code == sf::Keyboard::Num1 && bombAmmo > 0)
					{
						--bombAmmo;
						// add shoot bomb
					}

					// Reset stuff on new game
					if ((event.key.code == sf::Keyboard::P) && ( gameLost == true || gameWon == true) )
					{
						deflectorPosition = sf::Vector2f(275.f, 577.f);	//separate object that represents deflector position
						ballDeflector.setPosition(0, 0);	//Its origin position
						ballDeflector.setFillColor(sf::Color::Yellow);

						ballVector.clear();
						ballVector.resize(1);
						ballVector[0].setPosition(0, 0);
						ballVector[0].move(deflectorPosition.x, deflectorPosition.y - ballVector[0].getRadius() - 1);
						int randomAngle = resetAngleDist(ballRanDev);
						ballVector[0].setAngle(randomAngle * (2 * std::_Pi / 360));
						ballVector[0].stick();

						brickVector.clear();
						brickVector.resize(50);

						for (int row = 0; row < 5; ++row)
						{
							for (int column = 0; column < 10; ++column)
							{
								brickVector[(row * 10) + column].setPosition(sf::Vector2f((1 + column*55.f), (1 + row*19.f)));
								brickVector[(row * 10) + column].syncPowerUpPosition();
							}
						}

						gameLost = false;
						gameWon = false;
						brokenBricks = 0;
						deflectorMagnetized = false;
						bombAmmo = 0;
						numberOfBalls = 1;
						// add single ball revert
					}
				}
			}

		}

		

		//Moving the ball
		for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
		{
			if ((*ballIter).isDead() == false)
			{
				if (!(*ballIter).isStuck() && gameLost == false && gameWon == false && paused == false && startScreen == false)
				{
					(*ballIter).move(speed*(cos((*ballIter).getAngle())), -speed*(sin((*ballIter).getAngle())));

					// If ball hits borders
					if (borderLeftBound.contains(((*ballIter).getPosition().x - (*ballIter).getRadius()), (*ballIter).getPosition().y))
					{
						(*ballIter).setAngle(std::_Pi - (*ballIter).getAngle());
					}

					if (borderRightBound.contains(((*ballIter).getPosition().x + (*ballIter).getRadius()), (*ballIter).getPosition().y))
					{
						(*ballIter).setAngle(std::_Pi - (*ballIter).getAngle());
					}

					if (borderUpBound.contains(((*ballIter).getPosition().x), (*ballIter).getPosition().y - (*ballIter).getRadius()))
					{
						(*ballIter).setAngle(0 - (*ballIter).getAngle());
					}

					if (borderDownBound.contains(((*ballIter).getPosition().x), (*ballIter).getPosition().y + (*ballIter).getRadius()))
					{
						(*ballIter).kill();
						(*ballIter).move(0, 20);
						--numberOfBalls;

						if (numberOfBalls == 0)
						{
							gameLost = true;
						}
					}

					//Ball bouncing from deflector
					if (((*ballIter).getPosition().x >= (deflectorPosition.x - deflectorWidth / 2)) &&
						((*ballIter).getPosition().x <= (deflectorPosition.x + deflectorWidth / 2)) &&
						(((*ballIter).getPosition().y + (*ballIter).getRadius()) >= deflectorPosition.y) &&
						(((*ballIter).getPosition().y + (*ballIter).getRadius()) <= deflectorPosition.y + (0.75 * deflectorHight)) &&
						(gameLost == false))
					{
						//add sound here
						(*ballIter).setAngle(0 - (*ballIter).getAngle());

						// Make ball deflect more to the left if it hits the left edge of the deflector
						if (((*ballIter).getPosition().x >= (deflectorPosition.x - deflectorWidth / 2)) &&
							((*ballIter).getPosition().x <= (deflectorPosition.x - deflectorWidth / 4)))
						{
							(*ballIter).setAngle((*ballIter).getAngle() + std::_Pi / 6);
						}

						// Make ball deflect more to the right if it hits the right edge of the deflector
						if (((*ballIter).getPosition().x <= (deflectorPosition.x + deflectorWidth / 2)) &&
							((*ballIter).getPosition().x >= (deflectorPosition.x + deflectorWidth / 4)))
						{
							(*ballIter).setAngle((*ballIter).getAngle() - std::_Pi / 6);
						}

						// If deflector has the magnet powerUp
						if (deflectorMagnetized == true)
						{
							(*ballIter).stick();
						}
					}


					//Ball breaking bricks
					if (brokenBricks == 50)
					{
						gameWon = true;
					}
					for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
					{
						if ((*ballIter).isDead() == false)
						{
							bool foundBrick = false;	//To stop iterating once we have the impacted brick. Should get reinitialized on false every time "Move the ball" block runs
							for (auto brickIter = brickVector.begin(); brickIter != brickVector.end() && foundBrick == false; ++brickIter)
							{
								if ((*brickIter).isBroken() == false)
								{
									sf::FloatRect brickBound = (*brickIter).getGlobalBounds();

									for (auto ballPointIter = (*ballIter).getEdgePoints().begin(); ballPointIter != (*ballIter).getEdgePoints().end(); ++ballPointIter)
									{
										if (brickBound.contains(*ballPointIter))
										{
											//add brick crush sound here
											(*brickIter).crush();
											foundBrick = true;
											++brokenBricks;
											float ballSpeedRate = brokenBricks * 0.05; //To adjust ball speed to account for faster looping with less bricks. 
											(*ballIter).setSpeed(initialBallSpeed - ballSpeedRate);

											//Note: Brick origin is their top left corner, ball origin is its center
											//ball ricochet off of top side
											if (((*ballIter).getPosition().x >= (*brickIter).getPosition().x - (*ballIter).getRadius()) &&
												((*ballIter).getPosition().x <= ((*brickIter).getPosition().x + (*brickIter).getSize().x + (*ballIter).getRadius())) &&
												((*ballIter).getPosition().y < (*brickIter).getPosition().y))
											{
												(*ballIter).setAngle(0 - (*ballIter).getAngle());
											}

											//ball ricochet off of bottom side
											if (((*ballIter).getPosition().x >= (*brickIter).getPosition().x - (*ballIter).getRadius()) &&
												((*ballIter).getPosition().x <= ((*brickIter).getPosition().x + (*brickIter).getSize().x + (*ballIter).getRadius())) &&
												((*ballIter).getPosition().y > ((*brickIter).getPosition().y + (*brickIter).getSize().y)))
											{
												(*ballIter).setAngle(0 - (*ballIter).getAngle());
											}

											//ball ricochet off of left side
											if (((*ballIter).getPosition().y >= (*brickIter).getPosition().y - (*ballIter).getRadius()) &&
												((*ballIter).getPosition().y <= ((*brickIter).getPosition().y + (*brickIter).getSize().y + (*ballIter).getRadius())) &&
												((*ballIter).getPosition().x < (*brickIter).getPosition().x))
											{
												(*ballIter).setAngle(std::_Pi - (*ballIter).getAngle());
											}

											//ball ricochet off of right side
											if (((*ballIter).getPosition().y >= (*brickIter).getPosition().y - (*ballIter).getRadius()) &&
												((*ballIter).getPosition().y <= ((*brickIter).getPosition().y + (*brickIter).getSize().y + (*ballIter).getRadius())) &&
												((*ballIter).getPosition().x > ((*brickIter).getPosition().x + (*brickIter).getSize().x)))
											{
												(*ballIter).setAngle(std::_Pi - (*ballIter).getAngle());
											}

											break;	//Break out of checking the rest of the ball points on this brick since it broke already
										}
									}
								}
							}
						}
					}
				}
			}
		}
		

		//Move deflector
		if (gameLost == false && gameWon == false && paused == false && startScreen == false)
		{
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && (borderLeftBound.intersects(ballDeflector.getGlobalBounds()) == false))
			{
				ballDeflector.move(-deflectorSpeed, 0);
				deflectorPosition.x -= deflectorSpeed;
				for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
				{
					if ((*ballIter).isDead() == false)
					{
						if ((*ballIter).isStuck())
						{
							(*ballIter).move(-deflectorSpeed, 0);
						}
					}
				}
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && (borderRightBound.intersects(ballDeflector.getGlobalBounds()) == false))
			{
				ballDeflector.move(deflectorSpeed, 0);
				deflectorPosition.x += deflectorSpeed;
				for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
				{
					if ((*ballIter).isDead() == false)
					{
						if ((*ballIter).isStuck())
						{
							(*ballIter).move(deflectorSpeed, 0);
						}
					}
				}
			}
		}
		

		// Draw Start screen
		if (gameLost == false && gameWon == false && paused == false && startScreen == true)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);
			mainWindow.draw(startText);
			mainWindow.display();
		}

		// Draw gameplay
		if (gameLost == false && gameWon == false && paused == false && startScreen == false)
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
					if (theBrick.whichPower() != 0)
					{
						theBrick.drawItsPowerUp(mainWindow);

						if (theBrick.getPowerUpBounds().intersects(ballDeflector.getGlobalBounds()))
						{
							//add powerup to deflector
							//add powerUp gained sound
							switch (theBrick.whichPower())
							{
							case MAGNETIC: 
								deflectorMagnetized = true;
								ballDeflector.setFillColor(sf::Color(255, 0, 89));
								break;

							case BOMB: 
								++bombAmmo;
								break;

							case EXTRABALL: 
								ballVector.emplace_back();
								++numberOfBalls;
								(*(ballVector.end() - 1)).setPosition(0, 0);	// Set up the last ball made
								(*(ballVector.end() - 1)).move(deflectorPosition.x, deflectorPosition.y - (*(ballVector.end() - 1)).getRadius() - 1);
								(*(ballVector.end() - 1)).setAngle(resetAngleDist(ballRanDev) * (2 * std::_Pi / 360));
								(*(ballVector.end() - 1)).unStick();
								//add speed adjustment
								break;

							default: 
								cout << "powerUp consumption switch error." << endl;
								break;
							}

							theBrick.setPowerUpPosition(theBrick.getPowerUpPosition(), 0, 20);	//Make powerUp disappear offscreen when absorbed
						}

					}
				}
			}

			mainWindow.draw(ballDeflector);
			for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
			{
				if ((*ballIter).isDead() == false)
				{
					mainWindow.draw((*ballIter));
				}
			}
			mainWindow.display();
		}

		// Draw lose screen
		if (gameLost == true && gameWon == false && paused == false && startScreen == false)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);
			mainWindow.draw(loseText);
			mainWindow.draw(playAgain);
			mainWindow.display();
		}

		// Draw win screen
		if (gameLost == false && gameWon == true && paused == false && startScreen == false)
		{
			mainWindow.clear(sf::Color::Cyan);
			mainWindow.draw(background);
			mainWindow.draw(winText);
			mainWindow.draw(playAgain);
			mainWindow.display();
		}

		// Seems this block may be unnecessary. Check for cleaning at the end.
		/*
		// Draw pause screen
		if (gameLost == false && gameWon == false && paused == true && startScreen == false)
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
			mainWindow.draw(energyBall);
			mainWindow.draw(pauseText);
			mainWindow.display();
		}*/
	}

	
	return 0;
}

