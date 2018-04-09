#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <iostream>
#include <vector>
#include <functional>

void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

struct Particle : sf::Drawable
{
	Particle()
		: sprite{}
		, velocity{}
		, wholeLifetime{}
		, lifetime{}
	{
	}

	Particle(const sf::Texture *texture, sf::Vector2f position, sf::Vector2f velocity, sf::Time lifetime, float scale)
		: sprite{*texture}
		, velocity{velocity}
		, wholeLifetime{lifetime}
		, lifetime{lifetime}
	{
		centerOrigin(sprite);
		sprite.setPosition(position);
		sprite.setScale(scale, scale);
	}

	void reconstruct(const sf::Texture *texture, sf::Vector2f position, sf::Vector2f velocity, sf::Time lifetime, float scale)
	{
		sprite.setTexture(*texture);
		centerOrigin(sprite);
		sprite.setPosition(position);
		sprite.setScale(scale, scale);

		this->velocity = velocity;
		this->wholeLifetime = lifetime;
		this->lifetime = lifetime;
	}

	bool alive()
	{
		return lifetime > sf::Time::Zero;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite, states);
	}

	void update(sf::Time dt)
	{
		lifetime -= dt;
		if (lifetime < sf::Time::Zero) lifetime = sf::Time::Zero;
		sprite.move(velocity * dt.asSeconds());

		float ratio = lifetime / wholeLifetime;
		sf::Uint8 opacity = static_cast<sf::Uint8>(ratio * 255);

		sf::Color color = sprite.getColor();
		color.a = opacity;
		sprite.setColor(color);
	}

	sf::Sprite sprite;
	sf::Vector2f velocity;
	sf::Time wholeLifetime;
	sf::Time lifetime;
};

void randomParticleReconstruction(sf::Texture textures[], sf::Vector2f position, Particle &p)
{
	static std::mt19937 generator{};
	static std::uniform_int_distribution<int> textureDist{ 0, 8 };
	static std::uniform_real_distribution<float> velocityXDist{ -125.f, 125.f };
	static std::uniform_real_distribution<float> velocityYDist{ 200.f, 250.f };
	static std::uniform_real_distribution<float> lifetimeDist{ 0.5f, 1.5f };
	static std::uniform_real_distribution<float> scaleDist{ 0.2f, 0.22f };

	p.reconstruct(&textures[textureDist(generator)], position, { velocityXDist(generator), velocityYDist(generator) },
		sf::seconds(lifetimeDist(generator)), scaleDist(generator));
}

class Button : public sf::Drawable, public sf::Transformable
{
public:
	Button(sf::Vector2f size, const sf::Texture* texture, sf::String text, const sf::Font* font,
		sf::SoundBuffer& hoverBuffer, sf::SoundBuffer& clickBuffer)
		: m_size{size}
		, m_texture{texture}
		, m_label{text, *font, 20}
		, m_lastMousePos{}
		, m_callback{}
		, hoverSound{hoverBuffer}
		, clickSound{clickBuffer}
	{
		hoverSound.setVolume(5.f);
		hoverSound.setPitch(1.5f);

		clickSound.setVolume(50.f);
		clickSound.setPitch(1.5f);

		m_data[0].position = { 0.f, 0.f };
		m_data[1].position = { size.x, 0.f };
		m_data[2].position = { size.x, size.y };
		m_data[3].position = { 0.f, size.y };

		sf::Vector2u texSize = texture->getSize();

		m_data[0].texCoords = { 0.f, 0.f };
		m_data[1].texCoords = { (float)texSize.x, 0.f };
		m_data[2].texCoords = { (float)texSize.x, (float)texSize.y };
		m_data[3].texCoords = { 0.f, (float)texSize.y };

		centerOrigin(m_label);
		m_label.setPosition(size.x / 2.f, size.y / 2.f);
	}

	void handleEvent(const sf::Event &event)
	{
		switch (event.type)
		{
			case sf::Event::MouseMoved:
			{
				m_lastMousePos = { (float)event.mouseMove.x, (float)event.mouseMove.y };

				if (isInside(m_lastMousePos))
				{
					if (inside == false && hoverSound.getStatus() != sf::Sound::Playing)
					{
						inside = true;
						hoverSound.play();
					}


					setColor(hoverColor);
				}
				else
				{
					setColor(normalColor);
					inside = false;
				}
			} break;

			case sf::Event::MouseButtonReleased:
			{
				if (event.key.code == sf::Mouse::Left && isInside(m_lastMousePos))
				{
					m_callback();
					clickSound.play();
				}
			} break;
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.texture = m_texture;
		states.transform *= getTransform();
		target.draw(&m_data[0], 4, sf::Quads, states);
		target.draw(m_label, states);
	}

	void setColor(const sf::Color& color)
	{
		for (int i = 0; i < 4; i++)
			m_data[i].color = color;
	}

	void addCallback(std::function<void(void)> callback)
	{
		m_callback = callback;
	}

private:
	bool isInside(sf::Vector2f position) const
	{
		sf::FloatRect rect{ 0.f, 0.f, m_size.x, m_size.y };
		rect = getTransform().transformRect(rect);

		return position.x > rect.left && position.x < rect.left + rect.width 
			&& position.y > rect.top && position.y < rect.top + rect.height;
	}

	static const sf::Color normalColor;
	static const sf::Color hoverColor;

private:
	sf::Vector2f m_size;
	sf::Vertex m_data[4];
	const sf::Texture* m_texture;
	sf::Text m_label;

	sf::Vector2f m_lastMousePos;
	std::function<void(void)> m_callback;

	sf::Sound hoverSound;
	sf::Sound clickSound;
	bool inside = false;
};

const sf::Color Button::normalColor = sf::Color::White;
const sf::Color Button::hoverColor = sf::Color{ 100, 100, 100 };

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

	sf::Sprite spaceRocket{ spaceRocketTexture };
	sf::Text menuTitle{ "Yet Another Space Shooter", kenneyFuture, 40u };
	sf::RectangleShape buttonPanel{ {220.f, 140.f} };
	Button newGame{ {200.f, 40.f}, &buttonTexture, "New Game", &kenneyFuture, hoverSoundBuffer, clickSoundBuffer };
	Button continueGame{ {200.f, 40.f}, &buttonTexture, "Continue", &kenneyFuture, hoverSoundBuffer, clickSoundBuffer };
	Button exit{ { 200.f, 40.f }, &buttonTexture, "Exit", &kenneyFuture, hoverSoundBuffer, clickSoundBuffer };

	float starBackgroundVelocity = 50.f;

	sf::Sprite starBackground1{ starBackgroundTexture };
	sf::Sprite starBackground2{ starBackgroundTexture };

	starBackground2.move(0.f, -2560.f);

	std::vector<Particle> particles(500);
	for (auto& p : particles) randomParticleReconstruction(explosionTextures, { 400.f, 410.f }, p);

	// configuration
	centerOrigin(spaceRocket);
	spaceRocket.setPosition(400.f, 320.f);

	centerOrigin(menuTitle);
	menuTitle.setPosition(400.f, 30.f);

	buttonPanel.setPosition(90.f, 130.f);
	buttonPanel.setFillColor(sf::Color(150, 150, 150));
	buttonPanel.setOutlineColor(sf::Color::White);
	buttonPanel.setOutlineThickness(1.f);

	newGame.setPosition(100.f, 140.f);
	continueGame.setPosition(100.f, 180.f);
	exit.setPosition(100.f, 220.f);

	newGame.addCallback([] { std::cout << "New Game" << '\n';  });
	continueGame.addCallback([] { std::cout << "Continue" << '\n';  });
	exit.addCallback([&window] { std::cout << "Exit" << '\n'; 
		window.close();
	});

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else
			{
				newGame.handleEvent(event);
				continueGame.handleEvent(event);
				exit.handleEvent(event);
			}
		}

		dt += clock.restart();
		while (dt >= TimePerFrame)
		{
			for (auto& p : particles)
			{
				if (p.alive()) p.update(dt);
				else randomParticleReconstruction(explosionTextures, { 400.f, 410.f }, p);
			}

			starBackground1.move(0.f, starBackgroundVelocity * dt.asSeconds());
			starBackground2.move(0.f, starBackgroundVelocity * dt.asSeconds());

			if (starBackground1.getPosition().y >= 2560.f)
				starBackground1.move(0.f, 2 * -2560.f);

			if (starBackground2.getPosition().y >= 2560.f)
				starBackground2.move(0.f, 2 * -2560.f);

			dt -= TimePerFrame;
		}

		window.clear();
		for (auto& p : particles) window.draw(p);
		window.draw(starBackground1);
		window.draw(starBackground2);
		window.draw(spaceRocket);
		window.draw(menuTitle);
		window.draw(buttonPanel);
		window.draw(newGame);
		window.draw(continueGame);
		window.draw(exit);
		window.display();
	}

	return 0;
}