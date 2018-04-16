#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

struct ButtonDefinition
{
	sf::Vector2f size = { 100.f, 20.f };
	sf::Vector2f position = { 0.f, 0.f};
	sf::Color standardColor = { 255, 255, 255 };
	sf::Color hoverColor = { 100, 100, 100 };
	sf::Texture* texture = nullptr;

	sf::Font* font = nullptr;
	sf::String text = "";
	sf::Uint32 characterSize = 30;
	sf::Vector2f textOffset = { 0.f, 0.f };

	sf::SoundBuffer* hoverSoundBuffer = nullptr;
	sf::SoundBuffer* clickSoundBuffer = nullptr;
	float hoverSoundVolume = 100.f;
	float clickSoundVolume = 100.f;
	float hoverSoundPitch = 1.f;
	float clickSoundPitch = 1.f;

	std::function<void(void)> callback = [] { return; };
};

class Button : public sf::Drawable, public sf::Transformable
{
public:
	Button(sf::RenderWindow& window, const ButtonDefinition& def);

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setColor(const sf::Color& color);
	void setCallback(std::function<void(void)> callback);

private:
	bool isInside(const sf::Vector2f& position) const;

private:
	sf::RenderWindow& mWindow;

	sf::Vector2f mSize;
	sf::Color mStandardColor;
	sf::Color mHoverColor;
	const sf::Texture* mTexture;
	std::function<void(void)> mCallback;

	sf::Vertex mData[4];
	sf::Text mText;
	sf::Vector2f mTextOffset;
	sf::Sound mClickSound;
	sf::Sound mHoverSound;
	bool mHoverSoundPlayed;
};