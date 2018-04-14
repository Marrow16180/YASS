#pragma once

#include <SFML/Graphics.hpp>

struct Sprite : public sf::Drawable
{
	const sf::Texture* texture = nullptr;
	sf::IntRect textureRect = {};
	sf::Vertex vertices[4] = {};
	sf::Vector2f origin = {};
	sf::Vector2f position = {};
	sf::Vector2f scale = {1.f, 1.f};
	float rotation = 0.f;
	sf::Transform transform = sf::Transform::Identity;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};