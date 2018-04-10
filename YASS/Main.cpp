#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <iostream>
#include <vector>
#include <functional>

#include "Button.hpp"
#include "ParticleEmitter.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 640), "YASS");
	window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::Time dt = sf::Time::Zero;

	sf::Texture explosionTextures[9];
	sf::Texture spaceRocketTexture;
	sf::Font kenneyFuture;
	sf::Texture buttonTexture;
	sf::Texture starBackgroundTexture;
	sf::SoundBuffer hoverSoundBuffer;
	sf::SoundBuffer clickSoundBuffer;
	sf::Music music;
	music.openFromFile("Resources/theme.ogg");
	music.setVolume(50.f);
	music.setLoop(true);
	music.play();

	for (int i = 0; i <= 8; i++)
		if (!explosionTextures[i].loadFromFile("Resources/explosion0" + std::to_string(i) + ".png"))
			return EXIT_FAILURE;

	if (!spaceRocketTexture.loadFromFile("Resources/spaceRockets_002.png"))
		return EXIT_FAILURE;

	if (!kenneyFuture.loadFromFile("Resources/KenneyFuture.ttf"))
		return EXIT_FAILURE;

	if (!buttonTexture.loadFromFile("Resources/button.png"))
		return EXIT_FAILURE;

	if (!starBackgroundTexture.loadFromFile("Resources/starBackground.png"))
		return EXIT_FAILURE;

	if (!hoverSoundBuffer.loadFromFile("Resources/hover.ogg"))
		return EXIT_FAILURE;

	if (!clickSoundBuffer.loadFromFile("Resources/click.ogg"))
		return EXIT_FAILURE;

	ButtonDefinition buttonDef;

	buttonDef.size = { 200.f, 40.f };
	buttonDef.position = { 100.f, 140.f };
	buttonDef.standardColor = { 255, 255, 255 };
	buttonDef.hoverColor = { 100, 100, 100 };
	buttonDef.texture = &buttonTexture;
	buttonDef.font = &kenneyFuture;
	buttonDef.text = "New Game";
	buttonDef.characterSize = 20;
	buttonDef.textOffset = { 10.f, 0.f };
	buttonDef.hoverSoundBuffer = &hoverSoundBuffer;
	buttonDef.clickSoundBuffer = &clickSoundBuffer;
	buttonDef.hoverSoundVolume = 100.f;
	buttonDef.clickSoundVolume = 100.f;
    buttonDef.hoverSoundPitch = 1.f;
	buttonDef.clickSoundPitch = 1.f;
	buttonDef.callback = [] { std::cout << "New Game Callback!\n"; };
	Button buttonNewGame{ window, buttonDef };

	buttonDef.position = { 100.f, 180.f };
	buttonDef.text = "Continue";
	buttonDef.callback = [] { std::cout << "Continue Callback!\n"; };
	Button buttonContinue{ window, buttonDef };

	buttonDef.position = { 100.f, 220.f };
	buttonDef.text = "Exit";
	buttonDef.callback = [] { std::cout << "Exit Callback!\n"; };
	Button buttonExit{ window, buttonDef };

	// particle system test
	ParticleEmitterDefinition emitterDef;
	emitterDef.texture = &explosionTextures[0];
	emitterDef.particlesMaxCount = 2000;
	emitterDef.particlesPerSecond = 1000;

	emitterDef.initialPosition = { 400.f, 350.f };
	emitterDef.initialScale = { 0.1f, 0.1f };
	emitterDef.initialColor = sf::Color(255, 200, 200, 155);

	emitterDef.minLifetime = sf::seconds(0.9f);
	emitterDef.maxLifetime = sf::seconds(0.95f);

	emitterDef.direction = { 0.f, 1.f };
	emitterDef.minAngleOffset = -33.f;
	emitterDef.maxAngleOffset = 33.f;
	emitterDef.minLinearVelocity = 260.f;
	emitterDef.maxLinearVelocity = 265.f;
	emitterDef.minAngularVelocity = 260.f;
	emitterDef.maxAngularVelocity = 300.f;

	ParticleEmitter emitter{ emitterDef };
	emitter.setActive(true);

	sf::Sprite spaceRocket{ spaceRocketTexture };
	sf::Text menuTitle{ "Yet Another Space Shooter", kenneyFuture, 40u };
	sf::RectangleShape buttonPanel{ {220.f, 140.f} };

	float starBackgroundVelocity = 50.f;

	sf::Sprite starBackground1{ starBackgroundTexture };
	sf::Sprite starBackground2{ starBackgroundTexture };

	starBackground2.move(0.f, -2560.f);

	// configuration
	centerOrigin(spaceRocket);
	spaceRocket.setPosition(400.f, 320.f);

	centerOrigin(menuTitle);
	menuTitle.setPosition(400.f, 30.f);

	buttonPanel.setPosition(90.f, 130.f);
	buttonPanel.setFillColor(sf::Color(150, 150, 150));
	buttonPanel.setOutlineColor(sf::Color::White);
	buttonPanel.setOutlineThickness(1.f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else
			{
				buttonNewGame.handleEvent(event);
				buttonContinue.handleEvent(event);
				buttonExit.handleEvent(event);
			}
		}

		dt += clock.restart();
		while (dt >= TimePerFrame)
		{
			emitter.update(TimePerFrame);

			starBackground1.move(0.f, starBackgroundVelocity * TimePerFrame.asSeconds());
			starBackground2.move(0.f, starBackgroundVelocity * TimePerFrame.asSeconds());

			if (starBackground1.getPosition().y >= 2560.f)
				starBackground1.move(0.f, 2 * -2560.f);

			if (starBackground2.getPosition().y >= 2560.f)
				starBackground2.move(0.f, 2 * -2560.f);

			dt -= TimePerFrame;
		}

		window.clear();

		window.draw(starBackground1);
		window.draw(starBackground2);
		window.draw(emitter);
		window.draw(spaceRocket);
		window.draw(menuTitle);
		window.draw(buttonPanel);

		window.draw(buttonNewGame);
		window.draw(buttonContinue);
		window.draw(buttonExit);


		window.display();
	}

	return 0;
}