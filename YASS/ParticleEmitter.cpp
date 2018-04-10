#include "ParticleEmitter.hpp"

ParticleEmitter::ParticleEmitter(const ParticleEmitterDefinition & def)
	: mTexture{def.texture}
	, mMaxCount{def.particlesMaxCount}
	, mParticlesPerSecond{def.particlesPerSecond}
	, mParticles{}
	, mElapsedTime{sf::Time::Zero}
	, mActive{false}
	, mGenerator{}

	, mInitialPosition{def.initialPosition}
	, mInitialScale{def.initialScale}
	, mInitialColor{def.initialColor}

	, mDirection{def.direction}
	, mLifetimeDist{def.minLifetime.asSeconds(), def.maxLifetime.asSeconds()}
	, mAngleOffsetDist{def.minAngleOffset, def.maxAngleOffset}
	, mLinearVelocityDist{def.minLinearVelocity, def.maxLinearVelocity}
	, mAngularVelocityDist{def.minAngularVelocity, def.maxAngularVelocity}
{
	mParticles.reserve(def.particlesMaxCount);
}

void ParticleEmitter::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (auto& p : mParticles) if (p.alive()) target.draw(p, states);
}

void ParticleEmitter::update(sf::Time dt)
{
	if (mActive == false) return;

	mElapsedTime += dt;
	const sf::Time TimePerParticle = sf::seconds(1.f / mParticlesPerSecond);

	while (mElapsedTime >= TimePerParticle)
	{
		mElapsedTime -= TimePerParticle;

		if (mParticles.size() < mMaxCount)
		{
			mParticles.push_back(newParticle());
		}
		else // reuse old particles if possible
		{
			for (auto& p : mParticles)
			{
				if (!p.alive())
				{
					reconstructParticle(p);
					break;
				}
			}
		}
	}

	for (auto& p : mParticles) if (p.alive()) p.update(dt);
}

void ParticleEmitter::setActive(bool active)
{
	mActive = active;
}

Particle ParticleEmitter::newParticle()
{
	float rotation = 0.f;
	sf::Vector2f position = mInitialPosition;
	sf::Vector2f scale = mInitialScale;
	sf::Time lifetime = sf::seconds(mLifetimeDist(mGenerator));
	sf::Color color = mInitialColor;

	Particle p{ mTexture, rotation, position, scale, lifetime, color };

	float angularVelocity = mAngularVelocityDist(mGenerator);
	float linearVelocity = mLinearVelocityDist(mGenerator);
	float angleOffset = mAngleOffsetDist(mGenerator);
	sf::Vector2f direction = mDirection;
	rotateVector2f(direction, angleOffset);

	p.angularVelocity = angularVelocity;
	p.linearVelocity = linearVelocity;
	p.direction = direction;

	return p;
}

void ParticleEmitter::reconstructParticle(Particle & p)
{
	float rotation = 0.f;
	sf::Vector2f position = mInitialPosition;
	sf::Vector2f scale = mInitialScale;
	sf::Time lifetime = sf::seconds(mLifetimeDist(mGenerator));
	sf::Color color = mInitialColor;

	p.reconstruct(mTexture, rotation, position, scale, lifetime, color);

	float angularVelocity = mAngularVelocityDist(mGenerator);
	float linearVelocity = mLinearVelocityDist(mGenerator);
	float angleOffset = mAngleOffsetDist(mGenerator);
	sf::Vector2f direction = mDirection;
	rotateVector2f(direction, angleOffset);

	p.angularVelocity = angularVelocity;
	p.linearVelocity = linearVelocity;
	p.direction = direction;
}
