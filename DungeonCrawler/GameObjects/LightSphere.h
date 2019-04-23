#ifndef _LIGHT_SPHERE_H
#define _LIGHT_SPHERE_H
#include "GameObject.h"

class LightSphere : public GameObject {

public:

	LightSphere(Mesh* mesh, Type type, const glm::vec3& position);
	virtual ~LightSphere();

	void update(float dt);

private:
	Type m_type;
};

#endif
