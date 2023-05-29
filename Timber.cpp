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

		// Clear
		window.clear();

		// Draw
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