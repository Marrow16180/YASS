#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Button.hpp"
#include "ParticleEmitter.hpp"
#include "Slider.hpp"
#include "Starfield.hpp"

struct MenuResources
{
	sf::SoundBuffer hoverButton;
	sf::SoundBuffer clickButton;
	sf::Texture fire;
	sf::Texture rocket;
	sf::Texture button;
	sf::Texture bar;
	sf::Texture arrow;
	sf::Font font;

	MenuResources();
};

class Menu
{
public:
	static const sf::Time TimePerFrame;

public:
	Menu();
	Menu(const Menu& menu) = delete;
	void operator=(const Menu& menu) = delete;

	void run();

private:
	void startMusic();
	void handleEvent();
	void render();
	void update();

private:
	MenuResources mResources;
	sf::Music mMusic;

	sf::Clock mClock;
	sf::Time mDelta;

	sf::RenderWindow mWindow;

	std::unique_ptr<Starfield> mStarfield;
	std::unique_ptr<Slider> mVolumeSlider;
	std::unique_ptr<Button> mNewGameButton;
	std::unique_ptr<Button> mContinueButton;
	std::unique_ptr<Button> mOptionsButton;
	std::unique_ptr<Button> mExitButton;
	sf::RectangleShape mButtonPanel;
	std::unique_ptr<ParticleEmitter> mParticleEmitter;
	sf::Sprite mRocket;
	sf::Text mMenuTitle;
};