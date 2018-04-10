#pragma once

#include "Utility.hpp"

struct Particle : public sf::Drawable
{
	Particle();

	Particle
	(
		const sf::Texture* texture, 
		float rotation, 
		sf::Vector2f position, 
		sf::Vector2f scale, 
		sf::Time lifetime,
		sf::Color color
	);

	void reconstruct
	(
		const sf::Texture* texture,
		float rotation,
		sf::Vector2f position,
		sf::Vector2f scale,
		sf::Time lifetime,
		sf::Color color
	);

	bool alive() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time dt);

	sf::Sprite sprite;
	sf::Time initialLifetime;
	sf::Time lifetime;

	sf::Vector2f direction;
	float linearVelocity;
	float angularVelocity;
};