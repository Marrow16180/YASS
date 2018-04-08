#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 640), "YASS");
	sf::Texture explosions[9];
	sf::Texture spaceRocket;
	sf::Font kenneyFuture;

	for (int i = 0; i <= 8; i++)
		if (!explosions[i].loadFromFile("Resources/explosion0" + std::to_string(i) + ".png"))
			return EXIT_FAILURE;

	if (!spaceRocket.loadFromFile("Resources/spaceRockets_002.png"))
		return EXIT_FAILURE;

	if (!kenneyFuture.loadFromFile("Resources/KenneyFuture.ttf"))
		return EXIT_FAILURE;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();
	}

	return 0;
}