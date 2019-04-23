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
}

void Effects::createEmitter(const std::string& emitterName, const std::string& textureName, float sizeOfAnParticle)
{
	Emitter* e = new Emitter(m_GLinit, textureName, sizeOfAnParticle);
	m_emitters[emitterName] = e;
}

void Effects::update(float dt)
{
	for (const auto& map: m_emitters)
	{
		map.second->update(dt);
	}
}

void Effects::addParticles(const std::string& emitterName, const glm::vec3& pos, float speed, float lifetime, int numberOfParticles)
{
	if (m_emitters.find(emitterName) != m_emitters.end())
	{
		m_emitters[emitterName]->addParticle(pos, speed, lifetime, numberOfParticles);
	}
	else
	{
		LOG_ERROR("Can't find emitter with name: " + emitterName);
	}
}

const std::map<std::string, Emitter*>&  Effects::getEmitters() const
{
	return m_emitters;
}

const unsigned int Effects::getTotalAmountOfParticles() const
{
	unsigned int amount = 0;
	for (const auto& map : m_emitters)
	{
		amount += map.second->getNumberOfParticles();
	}

	return amount;
}

