#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

struct SliderDefinition
{
	sf::Vector2f position = { 0.f, 0.f };
	sf::Vector2f barSize = { 200.f, 10.f };
	sf::Vector2f arrowSize = { 15.f, 20.f };
	sf::Vector2f arrowOffset = { 0.f, -5.f };

	sf::Texture* barTexture = nullptr;
	sf::Texture* arrowTexture = nullptr;

	std::function<void(float)> callback = [](float){ return; };
};

class Slider : public sf::Drawable, public sf::Transformable
{
public:
	Slider(sf::RenderWindow& window, const SliderDefinition& def);

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::FloatRect getLocalBounds() const;
	void setValue(float value);

private:
	bool isNearby(const sf::Vector2f& position) const;
	void predictArrowPosition(float x);
	void setArrowPosition(float x);

private:
	sf::RenderWindow& mWindow;
	sf::Vector2f mBarSize;
	sf::Vector2f mArrowSize;
	sf::Vector2f mArrowOffset;

	std::function<void(float)> mCallback;

	const sf::Texture* mBarTexture;
	const sf::Texture* mArrowTexture;
	sf::Vertex mBarData[4];
	sf::Vertex mArrowData[4];

	bool mMoving;
};