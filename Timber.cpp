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

		window.display();
	}

	return 0;
}