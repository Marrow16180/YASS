#include "Button.hpp"
#include <iostream>

Button::Button(sf::RenderWindow& window, const ButtonDefinition& def)
	: mWindow{window}
    , mSize{def.size}
	, mStandardColor{def.standardColor}
	, mHoverColor{def.hoverColor}
	, mTexture{def.texture}
	, mCallback{def.callback}
	, mHoverSoundPlayed{false}
{
	setPosition(def.position);
	setColor(def.standardColor);

	sf::Vector2f size = def.size;
	mData[0].position = { 0.f, 0.f };
	mData[1].position = { size.x, 0.f };
	mData[2].position = { size.x, size.y };
	mData[3].position = { 0.f, size.y };

	if (def.texture != nullptr)
	{
		sf::Vector2u textureSize = def.texture->getSize();
		mData[0].texCoords = { 0.f, 0.f };
		mData[1].texCoords = { (float)textureSize.x, 0.f };
		mData[2].texCoords = { (float)textureSize.x, (float)textureSize.y };
		mData[3].texCoords = { 0.f, (float)textureSize.y };
	}

	if (def.font != nullptr)
	{
		mText.setFont(*def.font);
		mText.setString(def.text);
		mText.setCharacterSize(def.characterSize);

		sf::FloatRect bounds = mText.getLocalBounds();
		mText.setPosition(def.textOffset + sf::Vector2f(bounds.left, bounds.top));
		std::cout << bounds.left << ' ' << bounds.top << '\n';
	}

	if (def.clickSoundBuffer != nullptr)
	{
		mClickSound.setBuffer(*def.clickSoundBuffer);
		mClickSound.setVolume(def.clickSoundVolume);
		mClickSound.setPitch(def.clickSoundPitch);
	}

	if (def.hoverSoundBuffer != nullptr)
	{
		mHoverSound.setBuffer(*def.hoverSoundBuffer);
		mHoverSound.setVolume(def.hoverSoundVolume);
		mHoverSound.setPitch(def.hoverSoundPitch);
	}
}

void Button::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::MouseMoved:
		{
			if (isInside({ (float)event.mouseMove.x, (float)event.mouseMove.y }))
			{
				setColor(mHoverColor);
				if (!mHoverSoundPlayed)
				{
					mHoverSound.play();
					mHoverSoundPlayed = true;
				}
			}
			else
			{
				setColor(mStandardColor);
				mHoverSoundPlayed = false;
			}
		} 
		break;

		case sf::Event::MouseButtonPressed:
		{
			
		} 
		break;

		case sf::Event::MouseButtonReleased:
		{
			auto mousePos = sf::Vector2f(sf::Mouse::getPosition(mWindow));
			if (event.key.code == sf::Mouse::Left && isInside(mousePos))
			{
				mCallback();
				mClickSound.play();
			}
		} 
		break;
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// states.shader = ... // add a blurry wave shader when hovering with mouse on button
	states.texture = mTexture;
	states.transform *= getTransform();

	target.draw(mData, 4u, sf::Quads, states);
	target.draw(mText, states);
}

void Button::setColor(const sf::Color& color)
{
	for (int i = 0; i < 4; i++) mData[i].color = color;
}

void Button::setCallback(std::function<void(void)> callback) // should I pass it by reference?
{
	mCallback = callback;
}

bool Button::isInside(const sf::Vector2f & position) const
{
	sf::FloatRect rect = getTransform().transformRect(sf::FloatRect({ 0.f, 0.f }, mSize));
	return position.x > rect.left && position.x < rect.left + rect.width
		&& position.y > rect.top && position.y < rect.top + rect.height;
}
