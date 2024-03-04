#include "SFML/Graphics.hpp"
#include"SFML/Window.hpp"
#include "SFML/System.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>


using namespace std;
using namespace sf;

int score = 0;
int temp = 1;

class fire
{
public:
	Sprite shapefire;
	fire(Texture* texture, Vector2f pos)
	{
		this->shapefire.setTexture(*texture);
		this->shapefire.setScale(.045f, .045f);
		this->shapefire.setPosition(pos);
	}
};

class enemy
{

public:
	Sprite shape;
	int lives;
	int livesmax;
	enemy(Texture* texture, Vector2u windowSize)
	{
		this->livesmax = 1;
		this->lives = this->livesmax;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.2f, 0.2f);
		this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, rand() % windowSize.y - this->shape.getGlobalBounds().height);
	}
};

class ship
{
public:
	Sprite shape;
	Texture* texture;
	int lives;
	int livesmax;
	vector < fire>fires;
	ship(Texture* texture)
	{
		this->livesmax = 10;
		this->lives = this->livesmax;
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(.12f, .12f);
	}
};

class gift
{
public:
	Sprite shapegift;
	int lives;
	int livesmax;

	gift(Texture* texture, Vector2u windowSize)
	{
		this->livesmax = 1;
		this->lives = this->livesmax;
		this->shapegift.setTexture(*texture);
		this->shapegift.setScale(0.2f, 0.2f);
		this->shapegift.setPosition(windowSize.x - this->shapegift.getGlobalBounds().width, rand() % (int)(windowSize.y - this->shapegift.getGlobalBounds().height));
	}
};

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(1000, 700), "Space Shooting Game", Style::Default);
	window.setFramerateLimit(50);
	Sprite background;
	Texture backgroundtext;
	backgroundtext.loadFromFile("background.jpg");
	background.setTexture(backgroundtext);
	background.setTextureRect(IntRect(0, 0, 1000, 1000));
	Font font;
	font.loadFromFile("font.ttf");

	Texture shiptext;
	shiptext.loadFromFile("ships.png");

	Texture firetext;
	firetext.loadFromFile("fire.png");

	Texture enemytext;
	enemytext.loadFromFile("enemy.png");

	Texture gifttext;
	gifttext.loadFromFile("gift.png");

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(Color::Green);
	scoreText.setPosition(900.f, 0.f);

	Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setCharacterSize(50);
	gameoverText.setFillColor(Color::White);
	gameoverText.setPosition(400.f, window.getSize().y / 2);
	gameoverText.setString("Game Over!");

	Text stage2Text;
	stage2Text.setFont(font);
	stage2Text.setCharacterSize(30);
	stage2Text.setFillColor(Color::White);
	stage2Text.setPosition(400.f, 10.f);
	stage2Text.setString("Stage: 2");

	Text stage3Text;
	stage3Text.setFont(font);
	stage3Text.setCharacterSize(30);
	stage3Text.setFillColor(Color::White);
	stage3Text.setPosition(400.f, 10.f);
	stage3Text.setString("Stage: 3");

	Text wonText;
	wonText.setFont(font);
	wonText.setCharacterSize(50);
	wonText.setFillColor(Color::Red);
	wonText.setPosition(200.f, window.getSize().y / 2);
	wonText.setString("Congratulations You Won!");

	ship ship(&shiptext);
	int shoottimer = 30;
	if (score > 20)
		shoottimer = 10;
	Text lifeBox;
	lifeBox.setFont(font);
	lifeBox.setCharacterSize(25);
	lifeBox.setFillColor(Color::Red);

	int enemyspawntimer = 0;
	vector<enemy> enemies;

	int giftspawntimer = 0;
	vector<gift> gifts;

	cout << "Press ESC key to Exit";
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}
		}
		if (ship.shape.getPosition().x <= 0)
		{
			ship.shape.setPosition(0.f, ship.shape.getPosition().y);
		}
		else if (ship.shape.getPosition().x >= window.getSize().x - ship.shape.getGlobalBounds().width)
		{
			ship.shape.setPosition(0.f, ship.shape.getPosition().y);
		}
		else if (ship.shape.getPosition().y <= 0)
		{
			ship.shape.setPosition(ship.shape.getPosition().x, window.getSize().y - ship.shape.getGlobalBounds().height);
		}
		if (ship.shape.getPosition().y >= window.getSize().y - ship.shape.getGlobalBounds().height)
		{
			ship.shape.setPosition(ship.shape.getPosition().x, window.getSize().y - ship.shape.getGlobalBounds().height);
		}


		if (ship.lives > 0)
		{
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				ship.shape.move(0.f, 10.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				ship.shape.move(0.f, -10.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				ship.shape.move(10.f, 0.f);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				ship.shape.move(-10.f, 0.f);
			}
			if (shoottimer < 30)
			{
				++shoottimer;
			}
			if (Keyboard::isKeyPressed(Keyboard::Space) && shoottimer >= 30)
			{
				ship.fires.push_back(fire(&firetext, ship.shape.getPosition()));
				shoottimer = 0;
			}

			for (int t = 0; t < ship.fires.size(); t++)
			{
				ship.fires[t].shapefire.move(30.f, 0.f);
				if (ship.fires[t].shapefire.getPosition().y > window.getSize().y)
				{
					ship.fires.erase(ship.fires.begin() + t);
					break;
				}

				for (int i = 0; i < enemies.size(); i++)
				{
					if (ship.fires[t].shapefire.getGlobalBounds().intersects(enemies[i].shape.getGlobalBounds()))
					{
						if (enemies[i].lives <= 1)
						{
							score = score + temp;
							enemies.erase(enemies.begin() + i);
						}
						else
							enemies[i].lives--;

						ship.fires.erase(ship.fires.begin() + t);
						break;
					}
				}
			}
			if (enemyspawntimer < 30)
			{
				++enemyspawntimer;
			}
			if (enemyspawntimer >= 30)
			{
				enemies.push_back(enemy(&enemytext, window.getSize()));
				if (score > 40)
				{
					enemies.push_back(enemy(&enemytext, window.getSize()));
					enemies.push_back(enemy(&enemytext, window.getSize()));
					enemies.push_back(enemy(&enemytext, window.getSize()));
					enemies.push_back(enemy(&enemytext, window.getSize()));
					enemies.push_back(enemy(&enemytext, window.getSize()));
				}
				enemyspawntimer = 0;
			}

			if (score > 20)
			{
				if (giftspawntimer < 30)
				{
					++giftspawntimer;
				}
				if (giftspawntimer >= 30)
				{
					gifts.push_back(gift(&gifttext, window.getSize()));
					giftspawntimer = 0;
				}
			}
			for (int j = 0; j < enemies.size(); j++)
			{
				enemies[j].shape.move(-5.f, 0.f);
				if (score > 40)
					enemies[j].shape.move(-10.f, 0.f);

				if (enemies[j].shape.getPosition().x <= 0 - enemies[j].shape.getGlobalBounds().width)
				{
					enemies.erase(enemies.begin() + j);
					break;
				}
				if (enemies[j].shape.getGlobalBounds().intersects(ship.shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + j);

					ship.lives--;
					break;
				}
			}
			if (score > 20)
			{
				for (int i = 0; i < gifts.size(); i++)
				{
					gifts[i].shapegift.move(-5.f, 0.f);

					if (gifts[i].shapegift.getPosition().x <= 0 - gifts[i].shapegift.getGlobalBounds().width)
					{
						gifts.erase(gifts.begin() + i);
						if (score > 20)
						{
							if (shoottimer < 10)
							{
								++shoottimer;
							}
							if (Keyboard::isKeyPressed(Keyboard::Space) && shoottimer >= 10)
							{
								ship.fires.push_back(fire(&firetext, ship.shape.getPosition()));
								shoottimer = 0;
							}
						}
						break;
					}
					if (gifts[i].shapegift.getGlobalBounds().intersects(ship.shape.getGlobalBounds()))
					{
						gifts.erase(gifts.begin() + i);
						if (score > 20)
						{
							temp = 3;
							if (shoottimer < 10)
							{
								++shoottimer;
							}
							if (Keyboard::isKeyPressed(Keyboard::Space) && shoottimer >= 10)
							{
								ship.fires.push_back(fire(&firetext, ship.shape.getPosition()));
								shoottimer = 0;
							}
						}


						break;
					}
				}
			}



		}

		window.clear();
		window.draw(background);

		window.draw(ship.shape);

		for (int i = 0; i < ship.fires.size(); i++)
		{
			window.draw(ship.fires[i].shapefire);

		}

		for (int i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i].shape);
		}

		if (score > 20)
		{
			for (int i = 0; i < gifts.size(); i++)
			{
				window.draw(gifts[i].shapegift);
			}
		}
		if (score > 60)
		{
			ship.lives = 10;
			window.clear();
		}

		if (ship.lives == 0)
		{
			window.clear();
			window.draw(gameoverText);
		}

		lifeBox.setString("Life: " + to_string(ship.lives) + "/" + to_string(ship.livesmax));

		scoreText.setString("Score:" + to_string(score));

		window.draw(scoreText);
		window.draw(lifeBox);
		if (score > 20 && score < 40)
			window.draw(stage2Text);
		if (score > 40 && score < 60)
			window.draw(stage3Text);
		if (score > 60)
			window.draw(wonText);

		window.display();
	}
}
