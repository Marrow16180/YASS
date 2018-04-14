#include "Starfield.hpp"

Starfield::Starfield(sf::Vector2f size)
	: mShape{ size }
	, mShader{}
	, mTime{}
{
	if (sf::Shader::isAvailable())
	{
		if (!mShader.loadFromFile("Resources/Menu/Starfield.frag", sf::Shader::Fragment))
			throw std::runtime_error("Cannot open: Resources/Menu/Starfield.frag");

		mShader.setUniform("u_resolution", size);
		mShader.setUniform("u_time", 0.f);
	}
	else
		throw std::runtime_error("Shaders are not supported on this system!");
}

void Starfield::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.shader = &mShader;
	target.draw(mShape, states);
}

void Starfield::update(sf::Time dt)
{
	mTime += dt.asSeconds();
	mShader.setUniform("u_time", mTime);
}
