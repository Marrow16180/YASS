#pragma once

#include <SFML/Graphics.hpp>

class Starfield : public sf::Drawable
{
public:
	Starfield(sf::Vector2f size);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time dt);

private:
	sf::RectangleShape mShape;
	sf::Shader mShader;
	float mTime;
};
