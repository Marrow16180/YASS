#include "Particle.hpp"

Particle::Particle()
{
	// TODO: check whether this can be empty...
}

Particle::Particle
(
	const sf::Texture * texture, 
	float rotation, 
	sf::Vector2f position, 
	sf::Vector2f scale, 
	sf::Time lifetime, 
	sf::Color color
)
	: sprite{*texture}
	, initialLifetime{lifetime}
	, lifetime{lifetime}
{
	centerOrigin(sprite);
	sprite.setRotation(rotation);
	sprite.setPosition(position);
	sprite.setScale(scale);
	sprite.setColor(color);
}

void Particle::reconstruct(const sf::Texture * texture, float rotation, sf::Vector2f position, sf::Vector2f scale, sf::Time lifetime, sf::Color color)
{
	sprite.setTexture(*texture);
	initialLifetime = lifetime;
	this->lifetime = lifetime;
	centerOrigin(sprite);
	sprite.setRotation(rotation);
	sprite.setPosition(position);
	sprite.setScale(scale);
	sprite.setColor(color);
}

bool Particle::alive() const
{
	return lifetime > sf::Time::Zero;
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Particle::update(sf::Time dt)
{
	lifetime -= dt;
	sprite.move(direction * linearVelocity * dt.asSeconds());
	sprite.rotate(angularVelocity * dt.asSeconds());
}
