#pragma once

#include "Particle.hpp"
#include <random>
#include <vector>
#include <iostream>
#include <functional>

typedef std::function<void(Particle& p, sf::Time dt)> Effector;

struct ParticleEmitterDefinition
{
	const sf::Texture * texture = nullptr;
	sf::Uint32 particlesMaxCount = 0;
	sf::Uint32 particlesPerSecond = 0;

	// initial parameters
	sf::Vector2f initialPosition = { 0.f, 0.f };
	sf::Vector2f initialScale = { 1.f, 1.f };
	sf::Color initialColor = sf::Color::White;

	sf::Time minLifetime = sf::Time::Zero;
	sf::Time maxLifetime = sf::Time::Zero;

	sf::Vector2f direction = { 0.f, 0.f };
	float minAngleOffset = 0.f;
	float maxAngleOffset = 0.f;
	float minLinearVelocity = 0.f;
	float maxLinearVelocity = 0.f;
	float minAngularVelocity = 0.f;
	float maxAngularVelocity = 0.f;
};

class ParticleEmitter : public sf::Drawable, public sf::Transformable
{
public:
	ParticleEmitter(const ParticleEmitterDefinition& def);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Time dt);

	void setActive(bool active);
	void setEffector(Effector effector);

private:
	Particle newParticle();
	void reconstructParticle(Particle& p);

private:
	const sf::Texture * mTexture;
	sf::Uint32 mMaxCount;
	sf::Uint32 mParticlesPerSecond;
	std::vector<Particle> mParticles;
	sf::Time mElapsedTime;
	bool mActive;
	Effector mEffector;

	std::mt19937 mGenerator;

	// initial parameters
	sf::Vector2f mInitialPosition;
	sf::Vector2f mInitialScale;
	sf::Color mInitialColor;

	sf::Vector2f mDirection;
	std::uniform_real_distribution<float> mLifetimeDist;
	std::uniform_real_distribution<float> mAngleOffsetDist;
	std::uniform_real_distribution<float> mLinearVelocityDist;
	std::uniform_real_distribution<float> mAngularVelocityDist;
};