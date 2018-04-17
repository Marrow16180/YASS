#include "Slider.hpp"

Slider::Slider(sf::RenderWindow & window, const SliderDefinition & def)
	: mWindow{window}
	, mBarSize{def.barSize}
	, mArrowSize{def.arrowSize}
	, mArrowOffset{def.arrowOffset}
	, mCallback{def.callback}
	, mBarTexture{def.barTexture}
	, mArrowTexture{def.arrowTexture}
	, mMoving{false}
{
	sf::Vector2f barSize = def.barSize;

	mBarData[0].position = { 0.f, 0.f };
	mBarData[1].position = { barSize.x, 0.f };
	mBarData[2].position = { barSize.x, barSize.y };
	mBarData[3].position = { 0.f, barSize.y };

	if (def.barTexture != nullptr)
	{
		sf::Vector2f textureSize = (sf::Vector2f)def.barTexture->getSize();
		mBarData[0].texCoords = { 0.f, 0.f };
		mBarData[1].texCoords = { textureSize.x, 0.f };
		mBarData[2].texCoords = { textureSize.x, textureSize.y };
		mBarData[3].texCoords = { 0.f, textureSize.y };
	}

	sf::Vector2f arrowSize = def.arrowSize;

	mArrowData[0].position = { 0.f, 0.f };
	mArrowData[1].position = { arrowSize.x, 0.f };
	mArrowData[2].position = { arrowSize.x, arrowSize.y };
	mArrowData[3].position = { 0.f, arrowSize.y };

	for (int i = 0; i < 4; i++) mArrowData[i].position += def.arrowOffset;

	if (def.arrowTexture != nullptr)
	{
		sf::Vector2f textureSize = (sf::Vector2f)def.arrowTexture->getSize();
		mArrowData[0].texCoords = { 0.f, 0.f };
		mArrowData[1].texCoords = { textureSize.x, 0.f };
		mArrowData[2].texCoords = { textureSize.x, textureSize.y };
		mArrowData[3].texCoords = { 0.f, textureSize.y };
	}
}

void Slider::handleEvent(const sf::Event & event)
{
	switch (event.type)
	{
		case sf::Event::MouseMoved:
		{
			if (mMoving) predictArrowPosition(float(event.mouseMove.x + getOrigin().x));
		}
		break;

		case sf::Event::MouseButtonPressed:
		{
			auto mousePos = sf::Vector2f(sf::Mouse::getPosition(mWindow));
			if (event.key.code == sf::Mouse::Left && isNearby(mousePos))
			{
				predictArrowPosition(mousePos.x + getOrigin().x);
				mMoving = true;
			}
		}
		break;

		case sf::Event::MouseButtonReleased:
		{
			if (event.key.code == sf::Mouse::Left)
				mMoving = false;
		}
		break;
	}
}

void Slider::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	states.texture = mBarTexture;
	target.draw(mBarData, 4, sf::Quads, states);

	states.texture = mArrowTexture;
	target.draw(mArrowData, 4, sf::Quads, states);
}

sf::FloatRect Slider::getLocalBounds() const
{
	return sf::FloatRect(0.f, 0.f, mBarSize.x, mBarSize.y);
}

void Slider::setValue(float value)
{
	setArrowPosition(value * mBarSize.x / 100.f);
	mCallback(value);
}

bool Slider::isNearby(const sf::Vector2f & position) const
{
	float horizontalSensitivity = 1.01f;
	float verticalSensitivity = 1.02f;
	sf::FloatRect rect = getTransform().transformRect(sf::FloatRect({ 0.f, 0.f }, mBarSize));
	return position.x > rect.left / horizontalSensitivity && 
		position.x < (rect.left + rect.width) * horizontalSensitivity && 
		position.y > rect.top / verticalSensitivity && 
		position.y < (rect.top + rect.height) * verticalSensitivity;
}

void Slider::predictArrowPosition(float x)
{
	float left = getPosition().x;
	float right = left + mBarSize.x;
	float value;

	if (x < left)
		value = 0.f;
	else if (x > right)
		value = 100.f;
	else
		value = 100.f * (x - left) / (right - left);

	setArrowPosition(value * mBarSize.x / 100.f);
	mCallback(value);
}

void Slider::setArrowPosition(float x)
{
	mArrowData[0].position = { x, 0.f };
	mArrowData[1].position = { x + mArrowSize.x, 0.f };
	mArrowData[2].position = { x + mArrowSize.x, mArrowSize.y };
	mArrowData[3].position = { x, mArrowSize.y };

	for (int i = 0; i < 4; i++) mArrowData[i].position += mArrowOffset;
}
