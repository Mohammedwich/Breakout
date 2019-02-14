// Breakout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Brick.h"
#include "Ball.h"
#include "Bomb.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;


int main()
{

	std::uniform_int_distribution<int> resetAngleDist(45, 135);	// Used for setting ball starter angles

	sf::ContextSettings context;
	context.antialiasingLevel = 8;

	sf::RenderWindow mainWindow(sf::VideoMode(550, 600), "Breakout", sf::Style::Close | sf::Style::Titlebar, context);

	sf::SoundBuffer ballDeflectBuffer;
	if (!ballDeflectBuffer.loadFromFile("Ball deflector impact.wav"))
	{
		std::cout << "Failed to load for ballDeflectBuffer." << std::endl;
	}
	sf::Sound ballDeflectSound(ballDeflectBuffer);


	sf::SoundBuffer shootBuffer;
	if (!shootBuffer.loadFromFile("Breakout_Shoot.wav"))
	{
		std::cout << "Failed to load for shootBuffer." << std::endl;
	}
	sf::Sound shootSound(shootBuffer);


	sf::SoundBuffer powerUpBuffer;
	if (!powerUpBuffer.loadFromFile("Breakout_PowerUp.wav"))
	{
		std::cout << "Failed to load for powerUpBuffer." << std::endl;
	}
	sf::Sound powerUpSound(powerUpBuffer);


	sf::Texture galaxyTexture;
	if (!galaxyTexture.loadFromFile("Galaxy.jpg"))
	{
		cout << "Failed to load for galaxyTexture" << endl;
	}


	sf::Texture textureForBricks;
	if (!textureForBricks.loadFromFile("Rocky.jpg"))
	{
		cout << "Failed to load for textureForBricks" << endl;
	}

	// Making background
	sf::Sprite background;
	background.setTexture(galaxyTexture);
	background.setScale(2, 2);
	background.setColor(sf::Color(255, 255, 255, 220));

	// Setting game's borders
	sf::RectangleShape borderUp;
	borderUp.setSize(sf::Vector2f(550, -5) );
	borderUp.setPosition(sf::Vector2f(0, 0) );
	borderUp.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderUpBound = borderUp.getGlobalBounds();

	sf::RectangleShape borderDown;
	borderDown.setSize(sf::Vector2f(550, 5));
	borderDown.setPosition(sf::Vector2f(0, 600));
	borderDown.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderDownBound = borderDown.getGlobalBounds();

	sf::RectangleShape borderLeft;
	borderLeft.setSize(sf::Vector2f(-5, 600));
	borderLeft.setPosition(sf::Vector2f(0, 0));
	borderLeft.setFillColor(sf::Color::Transparent);
	sf::FloatRect borderLeftBound = borderLeft.getGlobalBounds();

	sf::RectangleShape borderRight;
	borderRight.setSize(sf::Vector2f(5, 600));
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
	float deflectorSpeed = 4;
	float initialDeflectorSpeed = deflectorSpeed;
	
	bool deflectorMagnetized = false;


	// The bricks and their positions on the window
	std::vector<Brick> brickVector(50);

	for (int row = 0; row < 5; ++row)
	{
		for (int column = 0; column < 10; ++column)
		{
			brickVector[(row * 10) + column].setPosition(sf::Vector2f( (1+ column*55.f), (1 + row*19.f) ));
			brickVector[(row * 10) + column].setTexture(& textureForBricks);
			brickVector[(row * 10) + column].syncPowerUpPosition();
		}
	}

	//The ball
	std::vector<Ball> ballVector(1);
	ballVector[0].move(deflectorPosition.x, deflectorPosition.y - ballVector[0].getRadius() - 1);//set initial position on top of deflector
	float ballSpeed = ballVector[0].getSpeed();
	float initialBallSpeed = ballSpeed;

	//Bomb
	Bomb gravityBomb;
	int bombAmmo = 0;
	double initialBombSpeed = 2.5;
	double bombSpeed = initialBombSpeed;


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
	startText.setPosition(40, 170);
	startText.setString("Use the left and right keys to move the deflector.\n"
						"Use the up key to release the ball.\n"
						"To pause and un-pause click the space bar.\n\n"

						"PowerUps: Extra Ball(White), Magnetic(Pink),\n"
						"Gravity bomb(Black)\n\n"

						"Press 'B' to fire gravity-bombs and 'B' again to detonate.\n"
						"You can't fire another bomb until the previous one stops\n"
						"sucking. You can store up to 3 bombs.\n\n"

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
	int numberOfBalls = 1;
	int movingBalls = 0;	// Used to adjust speeds to compensate for increased looping
	double powerUpFallFactor = 1;	// Used to adjust how fast powerUps fall

	//Time 
	sf::Clock bombClock;
	sf::Time timeSinceDenotation;
	int millisecondsperLoop = 11;	// If changed, adjust deflector, ball, powerUp, bomb speeds.
	sf::Clock threadTimer;	//Used to make every loop last the same amount of time

	


	while (mainWindow.isOpen())
	{
		threadTimer.restart();

		sf::Event event;


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
									(*ballIter).move(0, -( (*ballIter).getSpeed() + 0.5) );	// To avoid edge glide bug
									++movingBalls;
								}
							}
						}
					}
					
					if ( (event.key.code == sf::Keyboard::B) && (bombAmmo > 0) && (gravityBomb.isDetonated() == false) )
					{
						if (gravityBomb.isFlying() == false)
						{
							gravityBomb.setPosition(sf::Vector2f(deflectorPosition.x, deflectorPosition.y - 5));
							gravityBomb.shoot();
							shootSound.play();
						}
						else
						{
							gravityBomb.detonate();
							--bombAmmo;	// Reduce ammo here and in upperBound impact so ammo condition doesn't prevent detonation
							bombClock.restart();
						}
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
								brickVector[(row * 10) + column].setTexture(&textureForBricks);
								brickVector[(row * 10) + column].syncPowerUpPosition();
							}
						}

						gameLost = false;
						gameWon = false;
						brokenBricks = 0;

						deflectorMagnetized = false;
						deflectorSpeed = initialDeflectorSpeed;	

						numberOfBalls = 1;
						movingBalls = 0;
						ballSpeed = initialBallSpeed;

						bombSpeed = initialBombSpeed;
						gravityBomb.setPosition(-99, -99);
						gravityBomb.revertDetonation();
						gravityBomb.stopFlight();
						timeSinceDenotation = sf::seconds(0);
						bombAmmo = 0;
					}
				}
			}

		}


		//Moving the ball
		for (auto ballIter = ballVector.begin(); ballIter != ballVector.end(); ++ballIter)
		{
			if (brokenBricks == 50)
			{
				gameWon = true;
			}
			if ((*ballIter).isDead() == false)
			{
				if (!(*ballIter).isStuck() && gameLost == false && gameWon == false && paused == false && startScreen == false)
				{
					(*ballIter).move(ballSpeed*(cos((*ballIter).getAngle())), -ballSpeed*(sin((*ballIter).getAngle())));

					// If ball hits borders
					if (borderLeftBound.contains(((*ballIter).getPosition().x - (*ballIter).getRadius()), (*ballIter).getPosition().y))
					{
						(*ballIter).setAngle(std::_Pi - (*ballIter).getAngle());
						ballDeflectSound.play();
					}

					if (borderRightBound.contains(((*ballIter).getPosition().x + (*ballIter).getRadius()), (*ballIter).getPosition().y))
					{
						(*ballIter).setAngle(std::_Pi - (*ballIter).getAngle());
						ballDeflectSound.play();
					}

					if (borderUpBound.contains(((*ballIter).getPosition().x), (*ballIter).getPosition().y - (*ballIter).getRadius()))
					{
						(*ballIter).setAngle(0 - (*ballIter).getAngle());
						ballDeflectSound.play();
					}

					if (borderDownBound.contains(((*ballIter).getPosition().x), (*ballIter).getPosition().y + (*ballIter).getRadius()))
					{
						(*ballIter).kill();
						(*ballIter).move(0, 20);
						--numberOfBalls;
						--movingBalls;

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
						(*ballIter).setAngle(0 - (*ballIter).getAngle());
						ballDeflectSound.play();

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

						(*ballIter).move(0, -( (*ballIter).getSpeed() + 0.5) );

						// If deflector has the magnet powerUp
						if (deflectorMagnetized == true)
						{
							if ((*ballIter).isStuck() == false)
							{
								(*ballIter).stick();
								double angleAdjustment = (std::_Pi * 2 / 3) * ( (deflectorPosition.x - (*ballIter).getPosition().x ) / deflectorWidth );
								(*ballIter).setAngle( (std::_Pi / 2) + angleAdjustment);
								--movingBalls;
							}
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
							for (auto brickIter = brickVector.begin(); brickIter != brickVector.end(); ++brickIter)
							{
								if ((*brickIter).isBroken() == false)
								{
									sf::FloatRect brickBound = (*brickIter).getGlobalBounds();

									for (auto ballPointIter = (*ballIter).getEdgePoints().begin(); ballPointIter != (*ballIter).getEdgePoints().end(); ++ballPointIter)
									{
										if (brickBound.contains(*ballPointIter))
										{
											(*brickIter).crush();
											++brokenBricks;

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
						if ( (*ballIter).isStuck() == true && borderLeftBound.contains( ((*ballIter).getPosition().x - (*ballIter).getRadius() ), ( (*ballIter).getPosition().y) ) == false )
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
						if ((*ballIter).isStuck() == true && borderRightBound.contains(((*ballIter).getPosition().x + (*ballIter).getRadius()), ((*ballIter).getPosition().y)) == false)
						{
							(*ballIter).move(deflectorSpeed, 0);
						}
					}
				}
			}
		}
		
		
		//Bomb block
		if (gravityBomb.isDetonated() == true)
		{
			timeSinceDenotation = bombClock.getElapsedTime();
		}

		if (timeSinceDenotation.asSeconds() > 3 && gravityBomb.isFlying() == true && gravityBomb.isDetonated() == true )
		{
			gravityBomb.setPosition(-99, -99);
			gravityBomb.revertDetonation();
			gravityBomb.stopFlight();
			timeSinceDenotation = sf::seconds(0);
		}
		
		if (borderUpBound.contains(gravityBomb.getPosition().x, gravityBomb.getPosition().y + gravityBomb.getRadius()))
		{
			gravityBomb.detonate();
			--bombAmmo; // Ammo reduction on detonation to avoid B-press event's ammo condition fail to stop player from detonating
			bombClock.restart();
		}

		if (gravityBomb.isDetonated() == true )
		{
			for (auto brickIter = brickVector.begin(); brickIter != brickVector.end(); ++brickIter)
			{
				sf::FloatRect bombBound = gravityBomb.getGlobalBounds();

				if ( (*brickIter).isBroken() == false && bombBound.intersects((*brickIter).getGlobalBounds()))
				{
					(*brickIter).crush();
					++brokenBricks;
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
						theBrick.drawItsPowerUp(mainWindow, powerUpFallFactor);

						if (theBrick.getPowerUpBounds().intersects(ballDeflector.getGlobalBounds()))
						{
							powerUpSound.play();

							theBrick.setPowerUpPosition(theBrick.getPowerUpPosition(), 50, 60);	//Make powerUp disappear offscreen when absorbed

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
								// Setting up the new ball
								(*(ballVector.end() - 1)).setPosition(0, 0);	
								(*(ballVector.end() - 1)).move(deflectorPosition.x, deflectorPosition.y - (*(ballVector.end() - 1)).getRadius() - 1);
								(*(ballVector.end() - 1)).setAngle(resetAngleDist(ballRanDev) * (2 * std::_Pi / 360));
								(*(ballVector.end() - 1)).unStick();
								++movingBalls;
								break;

							default: 
								cout << "powerUp consumption switch error." << endl;
								break;
							}
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

			if (gravityBomb.isFlying())
			{
				gravityBomb.draw(mainWindow, bombSpeed);
			}

			mainWindow.display();
		}
		if (paused == true)
		{
			mainWindow.draw(pauseText);
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


		//Throttling thread to make loop time consistent
		double loopTime = threadTimer.getElapsedTime().asMilliseconds();
		std::floor(loopTime);
		if (loopTime < millisecondsperLoop)
		{
			int sleepDuration = millisecondsperLoop - loopTime;
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
		}
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
	// Join/end any running threads here.

	return 0;
}

