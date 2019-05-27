#ifndef _EFFECTS_H
#define _EFFECTS_H
#include "Emitter.h"
#include "AnimatedEmitter.h"
#include <map>
class Effects
{

public:

	Effects(GLinit* glInit);
	~Effects();

	void createEmitter(const std::string& emitterName, const std::string& textureName, float sizeOfAnParticle);
	void createAnimatedEmitter(const std::string& emitterName, const std::string& spritesheet, unsigned pxWidth, unsigned pxHeight, unsigned frames, float animDelay, bool billboarded, float sizeOfAnParticle);
	void update(float dt);
	// Non animated particles
	void addParticles(const std::string& emitterName, const glm::vec3& pos, const glm::vec3& velocity, float lifetime, int numberOfParticles = 1);

	// Animated particles has different add methods
	void addAnimParticle(const std::string& emitterName, const glm::vec3& pos, const glm::vec3& velocity, float lifetime, int numberOfParticles = 1);
	void addAnimParticle(const std::string& emitterName, const glm::vec3& pos, const glm::vec3& rotation, const glm::vec3& velocity, float lifetime, int numberOfParticles = 1);
	void addAnimParticle(const std::string& emitterName, GameObject* parent, const glm::vec3& offset, int numberOfParticles = 1);
	void addAnimParticle(const std::string& emitterName, GameObject* parent, const glm::vec3& offset, const glm::vec3& rotation, int numberOfParticles = 1);

	const std::map<std::string, Emitter*>& getEmitters() const;
	const std::map<std::string, AnimatedEmitter*>& getAnimatedEmitters() const;

	const unsigned int getTotalAmountOfParticles() const;


private:
	GLinit* m_GLinit;
	std::map<std::string, Emitter*> m_emitters;
	std::map<std::string, AnimatedEmitter*> m_animatedEmitters;

};


#endif
