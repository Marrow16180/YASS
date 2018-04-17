#include "Menu.hpp"

const sf::Time Menu::TimePerFrame = sf::seconds(1.f / 60.f);

Menu::Menu()
	: mResources{}
	, mMusic{}

	, mClock{}
	, mDelta{sf::Time::Zero}

	, mWindow{sf::VideoMode(800u, 640u), "YASS"}

	, mStarfield{nullptr}
	, mVolumeSlider{nullptr}
	, mNewGameButton{nullptr}
	, mContinueButton{nullptr}
	, mOptionsButton{nullptr}
	, mExitButton{nullptr}
	, mButtonPanel{{220.0, 180.f}}
	, mParticleEmitter{nullptr}
	, mRocket{mResources.rocket}
	, mMenuTitle{"Yet Another Space Shooter", mResources.font, 40u }
{
	mWindow.setKeyRepeatEnabled(false);
	startMusic();

	// Starfield config
	mStarfield = std::make_unique<Starfield>(sf::Vector2f(mWindow.getSize()));

	// Volume Slider config
	SliderDefinition sliderDef;
	sliderDef.position = { 400.f, 600.f };
	sliderDef.barSize = { 300.f, 10.f };
	sliderDef.arrowSize = { 15.f, 20.f };
	sliderDef.arrowOffset = { -7.5f, -5.f };
	sliderDef.barTexture = &mResources.bar;
	sliderDef.arrowTexture = &mResources.arrow;
	sliderDef.callback = [this](float volume) { mMusic.setVolume(volume); };

	mVolumeSlider = std::make_unique<Slider>(mWindow, sliderDef);
	centerOrigin(*mVolumeSlider);
	mVolumeSlider->setPosition(400.f, 600.f);
	mVolumeSlider->setValue(50.f);

	// Buttons config
	ButtonDefinition buttonDef;
	buttonDef.size = { 200.f, 40.f };
	buttonDef.standardColor = { 255, 255, 255 };
	buttonDef.hoverColor = { 100, 100, 100 };
	buttonDef.texture = &mResources.button;
	buttonDef.font = &mResources.font;
	buttonDef.characterSize = 20;
	buttonDef.textOffset = { 10.f, 0.f };
	buttonDef.hoverSoundBuffer = &mResources.hoverButton;
	buttonDef.clickSoundBuffer = &mResources.clickButton;
	buttonDef.hoverSoundVolume = 100.f;
	buttonDef.clickSoundVolume = 100.f;
	buttonDef.hoverSoundPitch = 1.f;
	buttonDef.clickSoundPitch = 1.f;

	// New game button
	buttonDef.position = { 100.f, 140.f };
	buttonDef.text = "New Game";
	buttonDef.callback = [] { std::cout << "New Game Callback!\n"; };
	mNewGameButton = std::make_unique<Button>(mWindow, buttonDef);

	// Continue button
	buttonDef.position = { 100.f, 180.f };
	buttonDef.text = "Continue";
	buttonDef.callback = [] { std::cout << "Continue Callback!\n"; };
	mContinueButton = std::make_unique<Button>(mWindow, buttonDef);

	// Options button
	buttonDef.position = { 100.f, 220.f };
	buttonDef.text = "Options";
	buttonDef.callback = [] { std::cout << "Options Callback!\n"; };
	mOptionsButton = std::make_unique<Button>(mWindow, buttonDef);

	// Exit button
	buttonDef.position = { 100.f, 260.f };
	buttonDef.text = "Exit";
	buttonDef.callback = [] { std::cout << "Exit Callback!\n"; };
	mExitButton = std::make_unique<Button>(mWindow, buttonDef);

	// Panel
	mButtonPanel.setPosition(90.f, 130.f);
	mButtonPanel.setFillColor(sf::Color(150, 150, 150));
	mButtonPanel.setOutlineColor(sf::Color::White);
	mButtonPanel.setOutlineThickness(1.f);

	// Particle Emitter config
	ParticleEmitterDefinition emitterDef;
	emitterDef.texture = &mResources.fire;
	emitterDef.particlesMaxCount = 2000;
	emitterDef.particlesPerSecond = 1000;

	emitterDef.initialPosition = { 400.f, 350.f };
	emitterDef.initialScale = { 0.1f, 0.1f };
	emitterDef.initialColor = sf::Color(255, 200, 200, 155);

	emitterDef.minLifetime = sf::seconds(0.9f);
	emitterDef.maxLifetime = sf::seconds(0.95f);

	emitterDef.direction = { 0.f, 1.f };
	emitterDef.minAngleOffset = -33.f;
	emitterDef.maxAngleOffset = 33.f;
	emitterDef.minLinearVelocity = 260.f;
	emitterDef.maxLinearVelocity = 265.f;
	emitterDef.minAngularVelocity = 260.f;
	emitterDef.maxAngularVelocity = 300.f;

	mParticleEmitter = std::make_unique<ParticleEmitter>(emitterDef);
	mParticleEmitter->setActive(true);
	mParticleEmitter->setEffector([](Particle& p, sf::Time dt)
	{
		float ratio = p.lifetime.asSeconds() / p.initialLifetime.asSeconds();
		auto opacity = sf::Uint8(255.f * ratio / p.linearVelocity * 100.f);
		auto color = p.sprite.getColor();
		color.a = opacity;
		p.sprite.setColor(color);
	});

	centerOrigin(mRocket);
	mRocket.setPosition(400.f, 320.f);

	centerOrigin(mMenuTitle);
	mMenuTitle.setPosition(400.f, 30.f);
}

void Menu::run()
{
	while (mWindow.isOpen())
	{
		handleEvent();
		update();
		render();
	}
}

void Menu::startMusic()
{
	const char* musicPath = "Resources/Menu/Theme.ogg";
	if (!mMusic.openFromFile(musicPath))
		throw std::runtime_error("Cannot open: " + std::string(musicPath));

	mMusic.setVolume(50.f);
	mMusic.setLoop(true);
	mMusic.play();
}

void Menu::handleEvent()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
		else if (event.type == sf::Event::Resized)
		{
			auto view = mWindow.getView();
			view.setSize(float(event.size.width), float(event.size.height));
			mWindow.setView(view);
		}
		else
		{
			mNewGameButton->handleEvent(event);
			mContinueButton->handleEvent(event);
			mOptionsButton->handleEvent(event);
			mExitButton->handleEvent(event);
			mVolumeSlider->handleEvent(event);
		}
	}
}

void Menu::render()
{
	mWindow.clear();

	mWindow.draw(*mStarfield);
	mWindow.draw(*mParticleEmitter);
	mWindow.draw(mRocket);
	mWindow.draw(mMenuTitle);

	mWindow.draw(mButtonPanel);
	mWindow.draw(*mNewGameButton);
	mWindow.draw(*mContinueButton);
	mWindow.draw(*mOptionsButton);
	mWindow.draw(*mExitButton);

	mWindow.draw(*mVolumeSlider);

	mWindow.display();
}

void Menu::update()
{
	mDelta += mClock.restart();
	while (mDelta >= TimePerFrame)
	{
		mParticleEmitter->update(TimePerFrame);
		mStarfield->update(TimePerFrame);
		mDelta -= TimePerFrame;
	}
}

MenuResources::MenuResources()
	: hoverButton{}
	, clickButton{}
	, fire{}
	, rocket{}
	, button{}
	, bar{}
	, arrow{}
	, font{}
{
	const char* hoverButtonPath = "Resources/Menu/Hover.ogg";
	const char* clickButtonPath = "Resources/Menu/Click.ogg";
	const char* firePath = "Resources/Menu/Fire.png";
	const char* rocketPath = "Resources/Menu/Rocket.png";
	const char* buttonPath = "Resources/Menu/Button.png";
	const char* barPath = "Resources/Menu/Bar.png";
	const char* arrowPath = "Resources/Menu/Arrow.png";
	const char* fontPath = "Resources/Menu/KenneyFuture.ttf";

	if (!hoverButton.loadFromFile(hoverButtonPath))
		throw std::runtime_error("Cannot open: " + std::string(hoverButtonPath));

	if (!clickButton.loadFromFile(clickButtonPath))
		throw std::runtime_error("Cannot open: " + std::string(clickButtonPath));
	
	if (!fire.loadFromFile(firePath))
		throw std::runtime_error("Cannot open: " + std::string(firePath));

	if (!rocket.loadFromFile(rocketPath))
		throw std::runtime_error("Cannot open: " + std::string(rocketPath));

	if (!button.loadFromFile(buttonPath))
		throw std::runtime_error("Cannot open: " + std::string(buttonPath));

	if (!bar.loadFromFile(barPath))
		throw std::runtime_error("Cannot open: " + std::string(barPath));

	if (!arrow.loadFromFile(arrowPath))
		throw std::runtime_error("Cannot open: " + std::string(arrowPath));

	if (!font.loadFromFile(fontPath))
		throw std::runtime_error("Cannot open: " + std::string(fontPath));
}
