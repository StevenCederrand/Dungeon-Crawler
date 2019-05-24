#include "Effects.h"
#include "System/LOG.h"
#include <GLM/gtx/norm.hpp>
#include "Globals/Paths.h"
#include "Utility/Randomizer.h"
#include "Utility/Camera.h"


Effects::Effects(GLinit* glInit)
{
	m_GLinit = glInit;
}

Effects::~Effects()
{
	for (const auto& map : m_emitters)
	{
		delete map.second;
	}
	m_emitters.clear();

	for (const auto& map : m_animatedEmitters)
	{
		delete map.second;
	}
	m_animatedEmitters.clear();
}

void Effects::createEmitter(const std::string& emitterName, const std::string& textureName, float sizeOfAnParticle)
{
	Emitter* e = new Emitter(m_GLinit, textureName, sizeOfAnParticle);
	m_emitters[emitterName] = e;
}

void Effects::createAnimatedEmitter(const std::string& emitterName, const std::string& spritesheet, unsigned pxWidth, unsigned pxHeight, unsigned frames, float animDelay, bool billboarded, float sizeOfAnParticle)
{
	AnimatedEmitter* ae = new AnimatedEmitter(m_GLinit, spritesheet, pxWidth, pxHeight, frames, animDelay, billboarded, sizeOfAnParticle);
	m_animatedEmitters[emitterName] = ae;
}

void Effects::update(float dt)
{
	for (const auto& map: m_emitters)
	{
		map.second->update(dt);
	}

	for (const auto& map : m_animatedEmitters)
	{
		map.second->update(dt);
	}
}

void Effects::addParticles(const std::string& emitterName, const glm::vec3& pos, const glm::vec3& velocity, float lifetime, int numberOfParticles)
{
	if (m_emitters.find(emitterName) != m_emitters.end())
	{
		m_emitters[emitterName]->addParticle(pos, velocity, lifetime, numberOfParticles);
	}
	else
	{
		LOG_ERROR("Can't find emitter with name: " + emitterName);
	}
}

void Effects::addAnimParticle(const std::string& emitterName, const glm::vec3& pos, const glm::vec3& velocity, float lifetime, int numberOfParticles)
{
	if (m_animatedEmitters.find(emitterName) != m_animatedEmitters.end())
	{
		m_animatedEmitters[emitterName]->addParticle(pos, velocity, lifetime, numberOfParticles);
	}
	else
	{
		LOG_ERROR("Can't find animated emitter with name: " + emitterName);
	}
}

void Effects::addAnimParticle(const std::string& emitterName, const glm::vec3& pos, const glm::vec3& rotation, const glm::vec3& velocity, float lifetime, int numberOfParticles)
{
	if (m_animatedEmitters.find(emitterName) != m_animatedEmitters.end())
	{
		m_animatedEmitters[emitterName]->addParticle(pos, rotation, velocity, lifetime, numberOfParticles);
	}
	else
	{
		LOG_ERROR("Can't find animated emitter with name: " + emitterName);
	}
}

void Effects::addAnimParticle(const std::string& emitterName, GameObject* parent, const glm::vec3& offset, int numberOfParticles)
{
	if (m_animatedEmitters.find(emitterName) != m_animatedEmitters.end())
	{
		m_animatedEmitters[emitterName]->addParticle(parent, offset, numberOfParticles);
	}
	else
	{
		LOG_ERROR("Can't find animated emitter with name: " + emitterName);
	}
}

void Effects::addAnimParticle(const std::string& emitterName, GameObject* parent, const glm::vec3& offset, const glm::vec3& rotation, int numberOfParticles)
{
	if (m_animatedEmitters.find(emitterName) != m_animatedEmitters.end())
	{
		m_animatedEmitters[emitterName]->addParticle(parent, offset, rotation, numberOfParticles);
	}
	else
	{
		LOG_ERROR("Can't find animated emitter with name: " + emitterName);
	}
}

const std::map<std::string, Emitter*>&  Effects::getEmitters() const
{
	return m_emitters;
}

const std::map<std::string, AnimatedEmitter*>& Effects::getAnimatedEmitters() const
{
	return m_animatedEmitters;
}

const unsigned int Effects::getTotalAmountOfParticles() const
{
	unsigned int amount = 0;
	for (const auto& map : m_emitters)
	{
		amount += map.second->getNumberOfParticles();
	}

	for (const auto& map : m_animatedEmitters)
	{
		amount += map.second->getNumberOfParticles();
	}

	return amount;
}

