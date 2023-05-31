// Includes
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

struct Cloud
{
	Sprite sprite;
	bool active;
	float speed;
};

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

	Cloud cloud1 = { cloudSprite1, false, 0.0f };
	Cloud cloud2 = { cloudSprite2, false, 0.0f };
	Cloud cloud3 = { cloudSprite3, false, 0.0f };
	Cloud clouds[] = { cloud1, cloud2, cloud3 };

	Clock clock;
	int score = 0;
	Text messageText;
	Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
	bool paused = true;

	while (window.isOpen())
	{
		/**
		 * Handle some input
		 */
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 6.0f;
		}


		// Update
		if (!paused)
		{
			Time dt = clock.restart();
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f) {
				paused = true;
				messageText.setString("Out of time!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(
					textRect.left + textRect.width / 2.0f, 
					textRect.top + textRect.height / 2);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}
			
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

			for (int i = 0; i < 3; i++) {
				Cloud aCloud = clouds[i];
				if (!aCloud.active)
				{
					srand((int)time(0) * (10 * (i + 1)));
					aCloud.speed = (rand() % 200);
					srand((int)time(0) * (10 * (i + 1)));
					float height = (rand() % 150);
					aCloud.sprite.setPosition(-200, height);
					aCloud.active = true;
				}
				else
				{
					aCloud.sprite.setPosition(
						aCloud.sprite.getPosition().x + (aCloud.speed * dt.asSeconds()),
						aCloud.sprite.getPosition().y);
					if (aCloud.sprite.getPosition().x > 1920)
					{
						aCloud.active = false;
					}
				}
				clouds[i] = aCloud;
			}

			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
		}
		
		// Draw
		window.clear();
		window.draw(backgroundSprite);
		for (int i = 0; i < 3; i++) {
			Cloud aCloud = clouds[i];
			window.draw(aCloud.sprite);
		}
		window.draw(treeSprite);
		window.draw(beeSprite);
		
		// Draw the HUD
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused)
		{
			window.draw(messageText);
		}

		window.display();
	}

	return 0;
}