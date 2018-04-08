#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <vector>

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

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 640), "YASS");
	sf::Clock clock;
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
	sf::Time dt = sf::Time::Zero;

	sf::Texture explosionTextures[9];
	sf::Texture spaceRocketTexture;
	sf::Font kenneyFuture;

	for (int i = 0; i <= 8; i++)
		if (!explosionTextures[i].loadFromFile("Resources/explosion0" + std::to_string(i) + ".png"))
			return EXIT_FAILURE;

	if (!spaceRocketTexture.loadFromFile("Resources/spaceRockets_002.png"))
		return EXIT_FAILURE;

	if (!kenneyFuture.loadFromFile("Resources/KenneyFuture.ttf"))
		return EXIT_FAILURE;

	sf::Sprite spaceRocket{ spaceRocketTexture };
	sf::Text menuTitle{ "Yet Another Space Shooter", kenneyFuture, 40u };

	std::vector<Particle> particles(500);
	for (auto& p : particles) randomParticleReconstruction(explosionTextures, { 400.f, 410.f }, p);

	// configuration
	centerOrigin(spaceRocket);
	spaceRocket.setPosition(400.f, 320.f);

	centerOrigin(menuTitle);
	menuTitle.setPosition(400.f, 30.f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		dt += clock.restart();
		while (dt >= TimePerFrame)
		{
			for (auto& p : particles)
			{
				if (p.alive()) p.update(dt);
				else randomParticleReconstruction(explosionTextures, { 400.f, 410.f }, p);
			}
			dt -= TimePerFrame;
		}

		window.clear();
		for (auto& p : particles) window.draw(p);
		window.draw(spaceRocket);
		window.draw(menuTitle);
		window.display();
	}

	return 0;
}