// Includes
#include <SFML/Graphics.hpp>

using namespace sf;

// This is where the game starts
int main() 
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Timber!", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture background;
	background.loadFromFile("graphics/background.png");

	Sprite backgroundSprite;
	backgroundSprite.setTexture(background);
	backgroundSprite.setPosition(0, 0);

	Texture tree;
	tree.loadFromFile("graphics/tree.png");

	Sprite treeSprite;
	treeSprite.setTexture(tree);
	treeSprite.setPosition(810, 0);

	Texture bee;
	bee.loadFromFile("graphics/bee.png");

	Sprite beeSprite;
	beeSprite.setTexture(bee);
	beeSprite.setPosition(0, 800);
	bool beeActive = false;
	float beeSpeed = 0.0f;

	Texture cloud;
	cloud.loadFromFile("graphics/cloud.png");
	Sprite cloudSprite1;
	Sprite cloudSprite2;
	Sprite cloudSprite3;
	cloudSprite1.setTexture(cloud);
	cloudSprite2.setTexture(cloud);
	cloudSprite3.setTexture(cloud);
	cloudSprite1.setPosition(0, 0);
	cloudSprite2.setPosition(0, 150);
	cloudSprite3.setPosition(0, 300);

	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	Clock clock;

	while (window.isOpen())
	{
		/**
		 * Handle some input
		 */
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Update
		Time dt = clock.restart();

		if (!beeActive)
		{
			// Seed the random number generator
			srand((int)time(0));
			beeSpeed = (rand() % 200) + 200;
			srand((int)time(0) * 10);
			float height = (rand() % 500) + 500;
			beeSprite.setPosition(2000, height);
			beeActive = true;
		}
		else 
		{
			beeSprite.setPosition(
				beeSprite.getPosition().x - (beeSpeed * dt.asSeconds()),
				beeSprite.getPosition().y);
			if (beeSprite.getPosition().x < -100)
			{
				beeActive = false;
			}
		}

		if (!cloud1Active)
		{
			srand((int)time(0) * 10);
			cloud1Speed = (rand() % 200);
			srand((int)time(0) * 10);
			float height = (rand() % 150);
			cloudSprite1.setPosition(-200, height);
			cloud1Active = true;
		}
		else
		{
			cloudSprite1.setPosition(
				cloudSprite1.getPosition().x + (cloud1Speed * dt.asSeconds()),
				cloudSprite1.getPosition().y);
			if (cloudSprite1.getPosition().x > 1920)
			{
				cloud1Active = false;
			}
		}

		if (!cloud2Active)
		{
			srand((int)time(0) * 20);
			cloud2Speed = (rand() % 200);
			srand((int)time(0) * 20);
			float height = (rand() % 150);
			cloudSprite2.setPosition(-200, height);
			cloud2Active = true;
		}
		else
		{
			cloudSprite2.setPosition(
				cloudSprite2.getPosition().x + (cloud2Speed * dt.asSeconds()),
				cloudSprite2.getPosition().y);
			if (cloudSprite2.getPosition().x > 1920)
			{
				cloud2Active = false;
			}
		}

		if (!cloud3Active)
		{
			srand((int)time(0) * 30);
			cloud3Speed = (rand() % 200);
			srand((int)time(0) * 30);
			float height = (rand() % 150);
			cloudSprite3.setPosition(-200, height);
			cloud3Active = true;
		}
		else
		{
			cloudSprite3.setPosition(
				cloudSprite3.getPosition().x + (cloud3Speed * dt.asSeconds()),
				cloudSprite3.getPosition().y);
			if (cloudSprite3.getPosition().x > 1920)
			{
				cloud3Active = false;
			}
		}

		// Draw
		window.clear();
		window.draw(backgroundSprite);
		window.draw(cloudSprite1);
		window.draw(cloudSprite2);
		window.draw(cloudSprite3);
		window.draw(treeSprite);
		window.draw(beeSprite);

		window.display();
	}

	return 0;
}