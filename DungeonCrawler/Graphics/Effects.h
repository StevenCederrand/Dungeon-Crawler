#ifndef _EFFECTS_H
#define _EFFECTS_H
#include "Emitter.h"
#include <map>
class Effects
{

public:

	Effects(GLinit* glInit);
	~Effects();

	void createEmitter(const std::string& emitterName, const std::string& textureName, float sizeOfAnParticle);
	void update(float dt);
	void addParticles(const std::string& emitterName, const glm::vec3& pos, float speed, float lifetime, int numberOfParticles = 1);
	const std::map<std::string, Emitter*>& getEmitters() const;
	const unsigned int getTotalAmountOfParticles() const;


private:
	GLinit* m_GLinit;
	std::map<std::string, Emitter*> m_emitters;

};


#endif
