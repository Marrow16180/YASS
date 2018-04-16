#include "Utility.hpp"

void rotateVector2f(sf::Vector2f & v, float angle)
{
	float length = sqrt(v.x * v.x + v.y * v.y);
	float oldAngle = atan(v.y / v.x);
	float newAngle = oldAngle + angle / 180.f * 3.1415f;

	v.x = cos(newAngle) * length;
	v.y = sin(newAngle) * length;
}
