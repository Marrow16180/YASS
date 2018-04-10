#include "Utility.hpp"

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

void rotateVector2f(sf::Vector2f & v, float angle)
{
	float length = sqrt(v.x * v.x + v.y * v.y);
	float oldAngle = atan(v.y / v.x);
	float newAngle = oldAngle + angle / 180.f * 3.1415f;

	v.x = cos(newAngle) * length;
	v.y = sin(newAngle) * length;
}
