// Includes
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

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

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// The book has this as 220, 20, but the branch size is actually 440x80
		// the point here is to center the origin of the sprite so I picked 40
		branches[i].setOrigin(220, 40);
	}

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

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side playerSide = side::LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;
	spriteAxe.setPosition(AXE_POSITION_LEFT, 830);

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;
	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		/**
		 * Handle some input
		 */
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 6.0f;

			// Hide the branches initially.
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			spriteRIP.setPosition(675, 2000);
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}
		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;

				score++;
				timeRemaining += (2 / score) + 0.15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;

				score++;
				timeRemaining += (2 / score) + 0.15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;
				chop.play();
			}
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
				outOfTime.play();
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

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);
				} 
				else
				{
					branches[i].setPosition(3000, height);
				}
			}
			// Send the log flying.
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}
			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);
				spritePlayer.setPosition(2000, 660);
				messageText.setString("SQUISHED!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(
					textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				death.play();
			}
		}
		
		// Draw
		window.clear();
		window.draw(backgroundSprite);
		for (int i = 0; i < 3; i++) {
			Cloud aCloud = clouds[i];
			window.draw(aCloud.sprite);
		}
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.draw(treeSprite);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
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

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
		case 0:
			branchPositions[0] = side::LEFT;
			break;
		case 1:
			branchPositions[0] = side::RIGHT;
			break;
		default:
			branchPositions[0] = side::NONE;
			break;
	}
}