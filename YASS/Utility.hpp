#pragma once

#include <SFML/Graphics.hpp>

template<class T>
void centerOrigin(T& object)
{
	sf::FloatRect bounds = object.getLocalBounds();
	object.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void rotateVector2f(sf::Vector2f& v, float angle);